#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

START_TEST(test_buffer_reads_never_exceed_declared_length)
{
    // Invariant: Buffer reads never exceed the declared length
    const char *payloads[] = {
        "A",                    // Valid input (1 character)
        "123456789",            // Boundary case (9 characters - exact buffer size)
        "12345678901234567890", // Exploit case (20 characters - 2x+ buffer overflow)
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" // 50 chars - massive overflow
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        int pipefd[2];
        pid_t pid;
        
        if (pipe(pipefd) == -1) {
            ck_abort_msg("Pipe failed");
        }
        
        pid = fork();
        if (pid == 0) {
            // Child process
            close(pipefd[0]); // Close read end
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[1]);
            
            // Write the payload to stdin via a temporary file
            FILE *input_file = tmpfile();
            if (input_file == NULL) {
                exit(EXIT_FAILURE);
            }
            fprintf(input_file, "%s\n", payloads[i]);
            rewind(input_file);
            dup2(fileno(input_file), STDIN_FILENO);
            fclose(input_file);
            
            // Execute the actual production code
            execl("./Ayudantías/Material extra/Ejercicios de C/3 problemas introductorios/Ejercicio 1/ej1",
                  "./Ayudantías/Material extra/Ejercicios de C/3 problemas introductorios/Ejercicio 1/ej1",
                  NULL);
            exit(EXIT_FAILURE); // execl failed
        } else if (pid > 0) {
            // Parent process
            close(pipefd[1]); // Close write end
            
            int status;
            waitpid(pid, &status, 0);
            
            // Check if process terminated normally (not by signal)
            if (WIFSIGNALED(status)) {
                ck_abort_msg("Buffer overflow detected with payload: %s (signal %d)", 
                           payloads[i], WTERMSIG(status));
            }
            
            close(pipefd[0]);
        } else {
            ck_abort_msg("Fork failed");
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_buffer_reads_never_exceed_declared_length);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}