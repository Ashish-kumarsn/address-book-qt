const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const User = require('../models/userModel');

const authService = {
  register: async ({ name, email, password, tenantId }) => {
    // Check if user already exists
    const existing = await User.findOne({ email });
    if (existing) {
      throw new Error('Email already registered');
    }

    // Hash password
    const hashedPassword = await bcrypt.hash(password, 10);

    // Create user
    const user = await User.create({
      name,
      email,
      password: hashedPassword,
      tenantId
    });

    return {
      id: user._id,
      name: user.name,
      email: user.email,
      tenantId: user.tenantId
    };
  },

  login: async ({ email, password }) => {
    // Find user
    const user = await User.findOne({ email });
    if (!user) {
      throw new Error('Invalid email or password');
    }

    // Check password
    const isMatch = await bcrypt.compare(password, user.password);
    if (!isMatch) {
      throw new Error('Invalid email or password');
    }

    // Generate JWT
    const token = jwt.sign(
      { id: user._id, tenantId: user.tenantId },
      process.env.JWT_SECRET,
      { expiresIn: '7d' }
    );

    return {
      token,
      user: {
        id: user._id,
        name: user.name,
        email: user.email,
        tenantId: user.tenantId
      }
    };
  }
};

module.exports = authService;