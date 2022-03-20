struct file_types
{
    int dir;
    int reg;
    int sock;
    int blk;
    int fifo;
    int chr;
    int lnk;
};

void summary_files_info(struct file_types *ftp)
{
    printf("DIR: %d| SOCK: %d| REG: %d| BLK: %d| FIFO: %d| LNK: %d| CHR: %d| \n", ftp->dir, ftp->sock, ftp->reg, ftp->blk, ftp->fifo, ftp->lnk, ftp->chr);
}
