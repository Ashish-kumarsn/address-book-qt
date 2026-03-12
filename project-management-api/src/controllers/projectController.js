const projectService = require('../services/projectService');

const projectController = {
  create: async (req, res) => {
    try {
      const project = await projectService.createProject({
        ...req.body,
        ownerId: req.user.id,
        tenantId: req.user.tenantId
      });
      res.status(201).json(project);
    } catch (err) {
      res.status(400).json({ message: err.message });
    }
  },

  getAll: async (req, res) => {
    try {
      const projects = await projectService.getAllProjects(req.user.tenantId);
      res.status(200).json(projects);
    } catch (err) {
      res.status(500).json({ message: err.message });
    }
  },

  getOne: async (req, res) => {
    try {
      const project = await projectService.getProjectById(
        req.params.id,
        req.user.tenantId
      );
      res.status(200).json(project);
    } catch (err) {
      res.status(404).json({ message: err.message });
    }
  },

  update: async (req, res) => {
    try {
      const project = await projectService.updateProject(
        req.params.id,
        req.user.id,
        req.body
      );
      res.status(200).json(project);
    } catch (err) {
      res.status(403).json({ message: err.message });
    }
  },

  delete: async (req, res) => {
    try {
      await projectService.deleteProject(req.params.id, req.user.id);
      res.status(200).json({ message: 'Project deleted successfully' });
    } catch (err) {
      res.status(403).json({ message: err.message });
    }
  }
};

module.exports = projectController;