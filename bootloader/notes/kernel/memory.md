# PAGING
 so we are going to create a map  of virtual page to physical frames

## GET ram size
do make this map we need a ram size 
## GET what KERNEL was already occupied and what else has been occupied 

## BUILD mapo



## write functions to allocate and free memory physical memroy

I have done this butt having issue where the bios e820 memory read is faulty

potential issues -
1)) not entire kernel is read  loaded in memory
2) some issue with reading e820




Okay so i thought that entire kernel is not loaded by bootloader since i ran `stats -c %s build/kernel.elf` and it returned me a big number which was coming out to be  77 sectors  so i was writing to load all thtese sector .
I look at how we can load since 1 cylinder only contains 17 sector , i tried writing code through GPT to load 100 kernel but it wasnt really loading . so i gave up for that day 

I gave it 1-2 days to settle down and today came back to the issue and tried debugging again and before commiting to implementing multi cyclinder load or multi phase load (loading remainng kernel aftter entiering long mode) I tried to check why the memory maop entries is so large , then with help chat gpt  which gave me some debugging ideas , i came to know that i was not initializing the memory map entires to 0 , normalling in a elf , when a programs loads elf it zero outts the bss memory , but ours was not , soi it had some random value and that lead to incorrect memory map entriess , that was the nail in the coffin after solving few more issues the PMM was finally running 


## optimize

