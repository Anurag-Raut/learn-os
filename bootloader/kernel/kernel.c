void main() {
  volatile char *v = (volatile char *)0xB8000;

  v[0] = 'H';
  v[1] = 0x07;

  v[2] = 'Q';
  v[3] = 0x07;

  while (1)
    ;
}
