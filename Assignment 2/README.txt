In order to compile the program there are two files required in the same directory. The movie_data.csv file must be in the same file as new.c. Additionally, you must ensure that you are in the OSU server OS1. 

This goes the same for other data csv used. They must all be in the same directory. 

Then to comile the program enter the following in the terminal:
>>make clean
>>make

Then to run with a certain data file enter the following:
>>./movies_by_year "name of csv".csv

for example with the data sample we were given:
>>./movies_by_year movies_sample.csv

Then program should run. If the program is run without the csv then a message will appear to  explain how to enter the csv file. Also, if the .csv is not included then a segmentation fault will occur. While the program used multiple files, the makefile runs them all as necessary. As there it runs the gcc command.
