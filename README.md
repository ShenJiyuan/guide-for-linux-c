# guide-for-linux-c

In this directory, I will put some simple linux C codes for you to get started. All codes were written when I was studying Operating System Class in SJTU including file copy, shell-like programing, threading and IPC problem. Welcome to give any suggestions and advice.

# list

1、MyCopy    ——Make a new copy of an existing file using system calls:
             ./MyCopy ***(your open file) ***(your write file)

2、ForkCopy  ——Make a new copy of an existing file using the MyCopy:
             ./ForkCopy ***(your open file) ***(your write file)

3、PipeCopy  ——Make a new copy of an existing file using pipe system calls:
             ./PipeCopy ***(your open file) ***(your write file)

4、use various system calls for time to compare the three versions of the file copy programs above.(Because ForkCopy uses the MyCopy to do the file copy, and there is also a time calculation program in the MyCopy. Then the time will be 2 values, one is for MyCopy, one is for ForkCopy.You can just see the second value.In fact, it would be better to just remove the time calculation program from MyCopy.c .) You can see the "copy_time.jpg".

5、MyShell   ——A shell-like program used to handle just "argument-less" 
             commands,such as ls and date.The sample terminal output is as 
             the "myshell.jpg":
             ./MyShell

6、MoreShell ——Make the mini-shell a little more powerful by allowing arguments
             to the commands,such as ls -l.The sample terminal output is as 
             the "moreshell.jpg":
             ./MoreShell

7、DupShell  ——Add to the mini-shell ability to execute command lines with
             commands connectedby pipes,such as ls -l | wc.The sample terminal
             output is as the "dupshell.jpg":
             ./DupShell

8、MergesortSingle  ——Implement mergesort using a single-threaded program:
                    ./MergesortSingle ***(your open file) ***(your write file)
   
   MergesortMulti   ——Implement mergesort using a multiple-threaded program:
                    ./MergesortMulti ***(your open file) ***(your write file)
 
   Compare the time, each program will return its time to the terminal, which is
   as the "mergesort.jpg".

9、BBC    ——Design,implement and test a solution for IPC problem, here for 
          BurgerBuddiesProblem:
          ./BBC cook_num cashier_num customer_num rack_size
