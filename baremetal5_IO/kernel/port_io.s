.global io_read32
io_read32:

  mov %di, %dx
  in %dx, %eax
ret

.global io_read8
io_read8:

  mov %di, %dx
  in %dx, %al
ret

.global io_write32
io_write32:
  mov %di, %dx
  mov %esi, %eax
  out %eax, %dx
ret

