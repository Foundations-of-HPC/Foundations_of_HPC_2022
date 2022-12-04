## install and run iozone

Visit the web site  (www.iozone.org)  and identify the latest tarball of the application

``` 
wget https://www.iozone.org/src/current/iozone3_494.tgz
tar -xvzf  iozone3_494.tgz
cd iozone3_494/src/current
```

Now run make
```

  $ make linux
  
```

and, after some warning, you will find the `iozone` executable in the directoy 

A few possible tests:
   
   - compile and run it on your laptop
   - run on /fast /scratch and /TEST_onp_run and compare results
  
