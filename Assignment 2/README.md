# Movie Data Processing Program

This program reads directory entries, selects a CSV file based on user criteria, processes the data, and creates new files to store the results.

The program should:
- Reads directory entries to find CSV files.
- Allows the user to select a file to process based on size or filename.
- Reads and processes movie data from the selected file.
- Creates a new directory with a randomly generated name.
- Generates and writes processed movie data into new files within the created directory.

**Columns in the CSV:**
- **Title**: A string representing the movie title (Example: `Iron Man 2`).
- **Year**: A four-digit integer representing the release year (Example: `2010`).
- **Languages**: Enclosed in `[]`, with multiple values separated by `;`. Maximum 5 languages, each up to 20 characters (Example: `[English;French]`).
- **Rating Value**: A number between 1 and 10, either an integer or a float with one decimal place (Example: `8.7`).
