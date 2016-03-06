# LimitSort
Sort a large file in a unit of line under the limit memory constrain

# Usage

    $ cd LimitSort

There are two exeution files, genData and main.

Step 1. generate data by genData

    $ make genData
    $ ./genData
    $ ./genData 16 4 ./dataFile
    $ cat ./dataFile

Step 2. sort ./dataFile by main

    $ make
    $ ./main
    $ ./main ./dataFile
    $ ls -t
    $ cat xxxx

The sorted output file is the lastest modified file, which is the first file listed in `ls -t`
