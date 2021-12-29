int ns0[] = {0x01, 0x02, 0x04, 0x08};
int ns4[] = {0x10, 0x20, 0x40, 0x80};

int shiftRight4(int n) {
    int out = 0;
    for(int i = 0; i < 4; i++) {
        if(n & ns4[i] > 0) {
            out |= ns0[i];
        }
    }
    return out;
}
