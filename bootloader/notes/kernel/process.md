Okay so I First Implemented  premeptive based scheduling and and then non preemtive
preemptive was easy , each process after ending would call the scheduler indicating they scehduler can switch to new process . It was easy because i just hadnt implemted the saving of context yet

So i just called scheduled at the end of each child process .
Then for preemtive i had to save resiters and stack point and then and keep track of what is the curent runnig process and keep on preemptive them every 100 ms based on the timer interrupt. This was the easy part, the difficult part was context switching , first i decided to store registers in the kernel stack (int the process struct )and then just keep on copy pasting them , basicaly popping them would restore them , but the later after too many failed attempts i ffound out that esp doesnt get copied , the stack pointer , so this would break , so the working  approach was to put the resiteres ont he stack on reach process itsself , but still it wasnt working , then i found the issue s, which was i was not creating the intial fake frame in the stack , so that was messing up the intitials load and then esp and it wasnt working .
Thats it , I am still not 100% sure about these approaches , i was very confused at that time and my head was hurting trying to debug , But i think a i have a gist  of it now , I want to read more about it to actually understand why it didnt worked . because everything is mush now , i tried lot of things
 



(

TODO : check how real scheduling works , when should we  context switch
TODO : Implement page table swtich
)


Question - where should the page table exits for a process , incide kernel space or user space?
A - process space 

Remove all print() calls from schedule() and timer interrupt handlers.
Create assembly ISR stubs for all CPU exceptions (0–31).
Handle exceptions with error codes (#PF, #GP, #SS, etc.) correctly.
Make all ISRs pass a consistent interrupt frame to the C handler.
Implement a proper page fault handler that prints CR2, EIP, CS, and the error code.
Add process states: NEW, READY, RUNNING, BLOCKED, TERMINATED.
Implement process_exit().
Add a task-return trampoline so finished tasks automatically call process_exit().
Allocate a dedicated kernel stack for every process.
Update the scheduler to switch to each process's kernel stack.
Stress-test context switching with multiple processes.
Verify address-space isolation by mapping the same virtual address to different physical pages in different processes.
Fix remaining compiler warnings (e.g., const warning in print_string()).
Remove or resolve the RWX ELF segment warning in the linker script (optional but recommended).


TODO imp next - 
Bootloader load more sectors or load it dynamic sector to load all kernel (Research on this)
