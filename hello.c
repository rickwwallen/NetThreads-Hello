//#include <stdio.h>
//#include <arpa/inet.h>
//#include <support.h>
//#include "common.h"
//#include "dev.h"

 #include "support.h"
 #include "pktbuff.h"

 #include "common.h"
 #include "dev.h"

 const char* mystr = "Hello world this is a test pretty awesome. This means that I can send more than just 64 Bytes I LOVE THIS!!!!!!";
 //const char* mystr = "I LOVE THIS!!!!!!";
 //const char* mystr = "Hello world!!!";
 #define STR_SIZE 512

 struct netfpga_to_driver 
 {
  char str[STR_SIZE];
 };
 
 #define PKT_SIZE (sizeof(struct ioq_header) + sizeof(struct netfpga_to_driver))

 int main() 
 {
   int mytid = nf_tid();
   
   if(mytid != 0)
     {
       nf_stall_a_bit();
       nf_lock(LOCK_INIT); // should not get it
     }
   else
     {
       nf_lock(LOCK_INIT); // should get it on the first attempt
       nf_pktout_init();
       nf_pktin_init();
     }
   nf_unlock(LOCK_INIT); 

   ////only run this program on thread 0
   //if (nf_tid() != 0) 
   //{
   //   while (1) {}
   //}

   //// initialize
   //nf_pktout_init();
   //nf_pktin_init();


   // allocate an output buffer
   t_addr *pkt = nf_pktout_alloc(PKT_SIZE);

   // setup the ioq_header
   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));

   // setup the ethernet header pointer
   //struct ioq_header *rioq = pkt_pull(&pkt, sizeof(struct ioq_header));

   // get a pointer to the payload struct
   struct netfpga_to_driver* n2d =   (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));

   // initialize the message
   memset(n2d->str, 0, STR_SIZE); 
   memcpy(n2d->str, mystr, strlen(mystr));
   n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
   //n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii

   // send it
   //pkt_push_all(&pkt);
   //nf_pktout_send(pkt.data, &(pkt.len));
   nf_pktout_send(pkt, pkt + PKT_SIZE); 
   

   //int i = 0;
   //int mytid;
   //struct netfpga_to_driver* n2d;
   //t_addr *pkt;
   //while(i <= 1)
   //{
   //   mytid = nf_tid();
   //   
   //   if(mytid != 0)
   //     {
   //       nf_stall_a_bit();
   //       nf_lock(LOCK_INIT); // should not get it
   //     }
   //   else
   //     {
   //       nf_lock(LOCK_INIT); // should get it on the first attempt
   //       nf_pktout_init();
   //       nf_pktin_init();
   //     }
   //   nf_unlock(LOCK_INIT); 

   //   // allocate an output buffer
   //   pkt = nf_pktout_alloc(PKT_SIZE);

   //   // setup the ioq_header
   //   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));

   //   // get a pointer to the payload struct
   //   n2d = 
   //     (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));

   //   // initialize the message
   //   memset(n2d->str, 0, STR_SIZE); 
   //   memcpy(n2d->str, mystr, strlen(mystr));
   //   //n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
   //   n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii

   //   // send it
   //   nf_pktout_send(pkt, pkt + PKT_SIZE); 
   //   i = i + 1;
   //}
//#else
//   int mytid = nf_tid();
//   
//   if(mytid != 0)
//     {
//       nf_stall_a_bit();
//       nf_lock(LOCK_INIT); // should not get it
//     }
//   else
//     {
//       nf_lock(LOCK_INIT); // should get it on the first attempt
//       nf_pktout_init();
//       nf_pktin_init();
//     }
//   nf_unlock(LOCK_INIT); 
//   t_addr *pkt = nf_pktout_alloc(PKT_SIZE);
//
//   // setup the ioq_header
//   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));
//
//   // get a pointer to the payload struct
//   struct netfpga_to_driver* n2d = 
//     (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));
//
//   // initialize the message
//   memset(n2d->str, 0, STR_SIZE); 
//   memcpy(n2d->str, mystr, strlen(mystr));
//   //n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
//   n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii
//   nf_pktout_send(pkt, pkt + PKT_SIZE); 
//#endif

   return 0;
 }
