const express = require('express');
const router = express.Router();
const projectController = require('../controllers/projectController');
const authMiddleware = require('../middleware/authMiddleware');
const { validateProject } = require('../middleware/validateMiddleware');

// All routes protected
router.use(authMiddleware);

router.post('/', validateProject, projectController.create);
router.get('/', projectController.getAll);
router.get('/:id', projectController.getOne);
router.put('/:id', validateProject, projectController.update);
router.delete('/:id', projectController.delete);

module.exports = router;