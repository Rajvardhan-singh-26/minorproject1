const express = require('express');
const path = require('path');
const { exec } = require('child_process');
const app = express();
const port = 3000;

// Middleware to parse JSON and URL encoded bodies
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Serve HTML file
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Handle form submission
app.post('/search', (req, res) => {
    const symptoms = req.body.symptoms;

    // Call the C++ executable with the symptoms as an argument
    exec(`./disease_search "${symptoms}"`, (err, stdout, stderr) => {
        if (err) {
            console.error('Error executing C++ program:', stderr);
            return res.status(500).send('Internal Server Error');
        }

        // Send the output back to the client
        res.send(`<h2>Search Results:</h2><pre>${stdout}</pre>`);
    });
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
