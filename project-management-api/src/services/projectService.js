const Project = require('../models/projectModel');

const projectService = {
  createProject: async ({ title, description, status, ownerId, tenantId }) => {
    const project = await Project.create({
      title,
      description,
      status,
      ownerId,
      tenantId
    });
    return project;
  },

  getAllProjects: async (tenantId) => {
    return await Project.find({ tenantId }).populate('ownerId', 'name email');
  },

  getProjectById: async (id, tenantId) => {
    const project = await Project.findOne({ _id: id, tenantId })
      .populate('ownerId', 'name email');
    if (!project) throw new Error('Project not found');
    return project;
  },

  updateProject: async (id, ownerId, updates) => {
    const project = await Project.findById(id);
    if (!project) throw new Error('Project not found');

    // Ownership check
    if (project.ownerId.toString() !== ownerId.toString()) {
      throw new Error('Unauthorized: You can only update your own projects');
    }

    const updated = await Project.findByIdAndUpdate(
      id,
      { ...updates },
      { new: true }
    );
    return updated;
  },

  deleteProject: async (id, ownerId) => {
    const project = await Project.findById(id);
    if (!project) throw new Error('Project not found');

    // Ownership check
    if (project.ownerId.toString() !== ownerId.toString()) {
      throw new Error('Unauthorized: You can only delete your own projects');
    }

    await Project.findByIdAndDelete(id);
    return true;
  }
};

module.exports = projectService;