const express = require('express');
const mongoose = require('mongoose');
require('dotenv').config();

const app = express();

const authRoutes = require('./routes/authRoutes');
const projectRoutes = require('./routes/projectRoutes');

app.use(express.json());

// Connect to MongoDB
mongoose.connect(process.env.MONGO_URI)
  .then(() => console.log('MongoDB connected successfully'))
  .catch((err) => console.error('MongoDB connection error:', err));

app.use('/api/auth', authRoutes);
app.use('/api/projects', projectRoutes);

app.get('/', (req, res) => {
  res.json({ message: 'Project Management API is running!' });
});

module.exports = app;