const authService = require('../services/authService');

const authController = {
  register: async (req, res) => {
    try {
      const user = await authService.register(req.body);
      res.status(201).json({ message: 'User registered successfully', user });
    } catch (err) {
      res.status(400).json({ message: err.message });
    }
  },

  login: async (req, res) => {
    try {
      const result = await authService.login(req.body);
      res.status(200).json(result);
    } catch (err) {
      res.status(401).json({ message: err.message });
    }
  }
};

module.exports = authController;