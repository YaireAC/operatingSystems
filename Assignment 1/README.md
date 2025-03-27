# Movie Data Processing Program

This program reads a CSV file containing movie data, processes the data into structs, stores them in a linked list, and provides an interactive menu for users to query information about the movies.

The program should:
- Read a **CSV file** with movie data provided as an argument.
- Process the file to **create structs** for each movie.
- Store the movie structs in a **linked list**.
- Provide an **interactive menu** for user queries about the movies.
- Display results based on user input.

The **CSV file** has a header row, followed by data for each movie. The file contains at least one movie, with no empty lines, and uses commas as delimiters. The file name is under 50 characters and contains no spaces.

**Columns in the CSV:**
- **Title**: A string (Example: `Iron Man 2`).
- **Year**: A four-digit integer (1900–2021) (Example: `2010`).
- **Languages**: Enclosed in `[]`, separated by `;`, max 5 languages, max 20 characters per language, case-sensitive (Example: `[English;Portuguese;Spanish]`).
- **Rating Value**: A number between 1 and 10, integer or float (1 decimal place) (Example: `8.7`).

When the program starts, it reads the file and prints:
Example output:
$ ./movies movies_sample_1.csv 
Processed file movies_sample_1.csv and parsed data for 24 movies

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 1
Enter the year for which you want to see movies: 1999
No data about movies released in the year 1999

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 1
Enter the year for which you want to see movies: 2012
The Avengers
Rise of the Guardians
Anna Karenina

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 2
2008 7.9 Iron Man
2009 7.6 Sherlock Holmes
2010 7.0 Iron Man 2
2013 7.2 Iron Man 3
2017 7.9 Thor: Ragnarok
2012 8.1 The Avengers
2016 7.8 Captain America: Civil War
2018 8.5 Avengers: Infinity War
2015 7.4 Avengers: Age of Ultron
2011 7.0 Thor
2014 7.8 Captain America: The Winter Soldier
2003 6.6 Right on Track

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 3
Enter the language for which you want to see movies: English
2008 The Incredible Hulk
2009 Sherlock Holmes
2008 Iron Man
2010 Iron Man 2
2013 Iron Man 3
2017 Thor: Ragnarok
2012 The Avengers
2016 Doctor Strange
2018 Avengers: Infinity War
2015 Avengers: Age of Ultron
2011 Thor
2013 Thor: The Dark World
2017 Spider-Man: Homecoming
2011 Captain America: The First Avenger
2016 Captain America: Civil War
2015 Ant-Man
2014 Captain America: The Winter Soldier
2018 Mary Queen of Scots
2016 Revolting Rhymes Part One
2017 The Glass Castle
2016 Free Fire
2003 Right on Track
2012 Rise of the Guardians
2012 Anna Karenina

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 3
Enter the language for which you want to see movies: Punjabi
No data about movies released in Punjabi

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 5
You entered an incorrect choice. Try again.

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 4
$
