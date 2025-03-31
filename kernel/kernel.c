void dummy_test_entrypoint(){

}

void print_string(const char* str) {
    char* vidmem = (char*) 0xb8000;
    int i = 0;
    while (str[i]) {
        vidmem[i * 2] = str[i];   // Character
        vidmem[i * 2 + 1] = 0x07; // Attribute (white text)
        i++;
    }
}


void main() {
    print_string("Hello from kernel");
}
