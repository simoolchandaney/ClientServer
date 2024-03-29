# Client Server

Group members:
- Ian Havenaar
- Simran Moolchandaney
- Jacob Schwartz

Assigned TAs:
- Alamin Mohammed
- Luke Siela

Due date: Friday, February 4th @ 10 PM

## Usage
- make (ran in ComputerNetworksProject01 directory): creates ./client/proj1client and ./server/proj1server executables
- ./project1client IP/hostname port file (ran under client subdirectory)
- ./proj1server port (ran under server subdirectory)
- make clean: removes executables 
        
  port has to be within 41000-41999 range and has to be the same for both commands

## Performance Metrics & Discussion
<img width="888" alt="Screen Shot 2022-02-03 at 9 06 13 PM" src="https://user-images.githubusercontent.com/72769479/152586714-64114d41-6799-4984-b97e-0d7839fa5cda.png">
We can observe that the average total transfer time is greater on local for the small file (which is why the average speed is smaller for when running on local host because time and speed are inversely proportional). We can observe that the opposite occurs for the large file.

## Extra Credit
- [X] Support the usage of a host name in addition to an IP address for the first argument of the client
- [X] Allow connections to arrive only from the 129.74.* address range, localhost (127.*), or the private network (192.168.*)
  

