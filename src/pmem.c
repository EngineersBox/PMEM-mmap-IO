#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libpmem.h>

/* using 1k of pmem for this example */
#define PMEM_LEN 1024

// Maximum length of our buffer
#define MAX_BUF_LEN 30

/****************************
 * This function writes the "Hello..." string to persistent-memory.
 *****************************/
void write_hello_string (char *buf, char *path) {
    char *pmemaddr;
    size_t mapped_len;
    int is_pmem;

    /* create a pmem file and memory map it */
    if ((pmemaddr = (char *) pmem_map_file(
        path,
        PMEM_LEN,
        PMEM_FILE_CREATE,
        0666,
        &mapped_len,
        &is_pmem
    )) == NULL) {
        perror("pmem_map_file");
        exit(1);
    }
    /* store a string to the persistent memory */
    strcpy(pmemaddr, buf);

    /* flush above strcpy to persistence */
    if (is_pmem)
        pmem_persist(pmemaddr, mapped_len);
    else
        pmem_msync(pmemaddr, mapped_len);

    /* output a string from the persistent memory to console */
    printf("\nWrite the (%s) string to persistent memory.\n",pmemaddr);
}

/****************************
 * This function reads the "Hello..." string from persistent-memory.
 *****************************/
void read_hello_string(char *path) {
    char *pmemaddr;
    size_t mapped_len;
    int is_pmem;

    /* open the pmem file to read back the data */
    if ((pmemaddr = (char *) pmem_map_file(
        path,
        PMEM_LEN,
        PMEM_FILE_CREATE,
        0666,
        &mapped_len,
        &is_pmem
    )) == NULL) {
        perror("pmem_map_file");
        exit(1);
    }
    /* Reading the string from persistent-memory and write to console */
    printf("\nRead the (%s) string from persistent memory.\n",pmemaddr);
}

/****************************
 * This main function gather from the command line and call the appropriate
 * functions to perform read and write persistently to memory.
 *****************************/
int main(int argc, char *argv[]) {
    char *path = argv[2];

    // Create the string to save to persistent memory
    char buf[MAX_BUF_LEN] = "Hello Persistent Memory!!!";

    if (strcmp (argv[1], "-w") == 0) {
        write_hello_string (buf, path);
    } else if (strcmp (argv[1], "-r") == 0) {
        read_hello_string(path);
    } else {
        fprintf(stderr, "Usage: %s <-w/-r> <filename>\n", argv[0]);
        exit(1);
    }

}