# C-8QueenRiddle-Threads-2-versions
This project is implemented by Threads in two versions:

  version 1:
  
    The main thread will create 15 + 15 + 8 + 8 = 46 threads.
    The work of testing the solution (a standard chessboard) will be shared between these threads.
    Eight threads (one per line) will check the line integrity of the board.
    A row is normal if it contains exactly one queen.
    Eight additional wires will check the integrity of the columns in the panel.
    A column is normal if it contains exactly one queen.
    The rest of the threads will check the diagonals in the panel, one for each thread.
    There are 15 diagonals that go from left to right, and 15 that go from right to left.
    A diagonal is correct if it contains at most one queen.
    The chessboard, which consists of dots and the letter Q (after being checked that it contains only "." and "Q" characters), and may     also contain whitespace, will be read by the main thread and will be stored in a data structure that all threads can access.
    The main thread will notify each thread that it creates what task it needs to do.
    For example, "Check the correctness of line # 3" by passing an appropriate argument.
    Each of the threads will notify the main thread of its test results.
    The main thread will wait for all threads to complete the test and then scan the results and come to a final conclusion as to           whether the solution is valid or not.
    This version does not use lock mutex or variables condition (according to the question guidelines).
    
  version 2:
  
      The main thread will check correctness of the testing solution (standard chess board).
      A valid chess board to check consists of only "Q","." and whitespace characters.
      A valid solutin for the test consists of exactly 1 Queen in each row/column and at most 1 Queen at the diagonals.
      The main thread will compile a list of 46 tasks to do("Check Line 1 Correctness," Check Line 2 Correctness, etc).
      The list of tasks will be saved in a simple data structure.
      After initializing the data structure in the to-do list, the main thread will create 4 threads.
      Each thread will go to the to-do list, "pull" a to-do list and execute it.
      The threads will continue to "retrieve" tasks from the list and execute them until the entire list is complete.
      All tasks must be performed exactly once.
      Unlike the first version, a single global int type variable, common to all threads, will be used to report test results.
      For example, the variable can hold the number of tests that ended correctly.
      Mutex (one or more) must be used to protect shared resources like To-do list and outcome variable.
      Variable condition must be used to report to the main thread when all assignments have ended and can check the result variable.
      In this version we will not use join to test the result but to release resources (according to the exercise guidelines).
