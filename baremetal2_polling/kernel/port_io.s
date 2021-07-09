.global io_read32
io_read32:

  mov %di, %dx
  in %dx, %eax
ret
