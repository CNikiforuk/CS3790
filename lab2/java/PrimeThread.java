package primethread;

import java.util.Arrays;


/*
	Author: Carlos Nikiforuk
	Lab Assignment #2

	Description: Program for using multiple threads to find prime numbers.

	Expected Use: prog <num primes> <num threads>
	
*/

public class PrimeThread implements Runnable {
    private String threadName;
    protected int[] interval = new int[2];
   
    /*==================================================*/
    PrimeThread(String name, int lower, int upper){
    //Constructor for PrimeThread object. Expects name, lower and upper limit.
    /*==================================================*/
       threadName =  name;
       interval[0] = lower;
       interval[1] = upper;
       System.out.println("Thread " + name + " created!");
    }
    
    
    /*==================================================*/
    boolean isPrime(int n){
    //returns true if passed value is a prime number false if it is a composite number.
    /*==================================================*/
             if (n <= 3) { return n > 1; } 
	else if (n % 2 == 0 || n % 3 == 0) { return false;}  
	else {
        for (int i = 5; i < Math.sqrt(n) + 1; i += 6){
            if (n % i == 0 || n % (i + 2) == 0) { return false; }
            }
        return true;
        }
    }
    
    
    /*==================================================*/
    public void run() {
    //Function executed by threads.
    /*==================================================*/
        for(int i=interval[0];i<interval[1];i++){
		if(isPrime(i)) System.out.printf("%s: %d \n",this.threadName, i);
	}
    }
    
    
    /*==================================================*/
    public static void main(String args[]) {
    //Main
    /*==================================================*/
	if(args.length != 2){
            System.err.printf("Usage: PrimeThread <upper limit> <num threads>\n");
        }        

	System.out.println("Searching for primes up to " + Integer.parseInt(args[0]) + "\n");
        
        int numthread;
        if (Integer.parseInt(args[0]) < Integer.parseInt(args[1])){ 
            numthread = Integer.parseInt(args[0])/2;
        }
	else {
            numthread = Integer.parseInt(args[1]); 
        }
        
        int interval = Integer.parseInt(args[0]) / numthread;	//how many numbers for each thread to process
	int remainder = Integer.parseInt(args[0]) % numthread;	//remainder for when (num % threads) != 0.
        int start = 0;                                          //each intervals start point
        
        for(int i=0;i<numthread;i++){
        String name = "t" + String.valueOf(i);
        (new Thread(new PrimeThread(name, start, start+interval))).start();
        start = start+interval;
        }
    }

}
