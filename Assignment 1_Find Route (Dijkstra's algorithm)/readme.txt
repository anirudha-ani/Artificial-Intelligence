Name:Anirudha Paul
ID: 1410091042

How to Run the Code:

Compiler Side:
This code is written in standard C++ with all standard library fucntions and header files. 
So either GNU G++ 5.1.0 or GNU G++11 5.1.0 compiler should be able to compile and run this code .

Input :

Reference Input Format is used.

Input format :  After Compiling from CMD type 

		<executable name> interactive <inputfile name > [computer-next/human-next] [depth]
			or
		<executable name> one-move <inputfile name > <output file name> [depth]

Example Input:

	"maxconnect4 interactive input1.txt computer-next 7"  
  		or
    	"maxconnect4 one-move red_next.txt green_next.txt 5" 

Warning : 
	1.This program is case sensitive . So please input names according to your given inputfile . Otherwise it won't work
	2.This program will give no warning message if the given inputfile don't exist in the directory . 
	  but that's not the case for output file . 
	3. Human player will get a warning for invalid move

Execution Time:

Depth   Time
1	0s
2	0s
3	0s
4	0s
5	.003s
6	.004s
7	.140s
8	.188s
9	2.068s
10	3.900s
11	1m 5.192s