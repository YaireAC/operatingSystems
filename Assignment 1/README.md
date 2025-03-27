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
