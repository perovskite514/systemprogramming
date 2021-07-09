#include "hardware.h"
#include "segmentation.h"
#include "sched.h"
#include "interrupt.h"
#include "memory.h"
#include "util.h"

#pragma GCC diagnostic ignored "-Wunused-parameter" // For l6 (kernel_param_dummy)

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info, unsigned long long kernel_param_dummy) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  init_virtual_memory();
  init_intr();
  init_frame_buffer(&hardware_info.fb);
  //puts("aaabbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  //puts("ababab\n");
  //puts("hgfjjodif\n");
  //puth(0xab, 4);
  puth(kernel_param_dummy, 10);
  // To here - Put this part at the top of start() function

  // Delete me. I'm a sample code.
  


      // † AYAME †



  // To here - sample code

  // Do not delete it!
  while (1);
}
