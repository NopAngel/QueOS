
void sys_print(const char* msg) {
    asm volatile("int $0x80" :: "a"(1), "b"(msg));
}

int main();

void _start() {
    int ret = main();
    // for (;;); 
    int SYS_exit = 60;
    asm volatile("int $0x80" :: "a"(SYS_exit), "b"(ret));
    for (;;); // should never be reached
}

int main(){
    sys_print(" message from app1\n");
    int x=0;
    for(int i=0;i<5;i++){
        sys_print(" app1 working...\n");
        x+=2;
    }

    return 0;
}