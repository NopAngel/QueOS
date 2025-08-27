
void sys_print(const char* msg) {
    asm volatile("int $0x80" :: "a"(1), "b"(msg));
}

void main(){
    sys_print(" message from app1\n");
    for(int i=0;i<3;i++){
        sys_print(" app1 working...\n");
    }
}