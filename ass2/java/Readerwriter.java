
package readerwriter;

import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;


/*
	Author: Carlos Nikiforuk
	Assignment #2

	Description: Reader writer example solution.

	Expected Use: prog <num primes> <num threads>
	
*/

public class Readerwriter implements Runnable{
    private String threadName;
    private boolean reader;
    private static final Semaphore count = new Semaphore(1, true);
    private static final Semaphore sem = new Semaphore(1, true);
    private static int readers;
    
    /*==================================================*/
    Readerwriter(String name, boolean r){
    //Constructor for Readerwriter object.
    /*==================================================*/
       threadName =  name;
       reader = r;
       System.out.println(name + ": Created!");
    }
    
    /*==================================================*/
    public void reader() throws InterruptedException{
    //Function executed by reader threads.
    /*==================================================*/
        count.acquire();
        readers++;
        if(readers == 1) sem.acquire();
        count.release();
        
        System.out.printf("%s: Reading...\n", threadName);
        Thread.sleep((long)Math.random()*2+1);
        System.out.printf("%s: Done reading!\n", threadName);
        
        count.acquire();
        readers--;
        if(readers == 0) { sem.release(); System.out.printf("\n"); }
        count.release();
    }
    
    /*==================================================*/
    public void writer() throws InterruptedException{
    //Function executed by writer threads.
    /*==================================================*/
        sem.acquire();

	System.out.printf("%s: Writing...\n", threadName);		
	Thread.sleep((long)Math.random()*3+1);
	System.out.printf("%s: Done writing!\n", threadName);
	
	sem.release();
    }  
    
    /*==================================================*/
    public void run() {
    //Run thread. Boolean reader determines whether thread is r or w.
    /*==================================================*/
        if(reader){
            try {
                reader();
            } catch (InterruptedException ex) {}
        }
        else {
            try {
                writer();
            } catch (InterruptedException ex) {}
        }
    }
    
    /*==================================================*/
    public static void main(String[] args) {
    //Main
    /*==================================================*/
        
        int numthread = (int)(Math.random()*5+4);
        Thread t[] =  new Thread[numthread];
        
        for(int i=0;i<numthread;i++){
            String name = "t" + String.valueOf(i);
            boolean r;
            if (Math.random()*2 >= 1) r = true;
            else r = false;
            t[i] = new Thread(new Readerwriter(name, r));
            t[i].start();
        }
        
        
        
    }
    
     
}
