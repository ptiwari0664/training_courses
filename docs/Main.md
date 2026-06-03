# Week-wise Execution Plan

This document outlines the planned weekly curriculum for the Robotics PoC Enablement Program. Each week lists learning objectives, practical outcomes, and links to the supporting lesson files for Week 1 and Week 2.

## Week 1 — ROS 2 and MoveIt Foundation
- Topics:
	- ROS 2 nodes, topics, services, actions, and TF
	- Launch files and simulation communication workflows
	- Sensor interfaces and `ros2_control` integration basics
	- MoveIt semantic robot model and planning groups (SRDF)
	- Kinematics, planners, and the planning/collision scene
	- Trajectory execution, time-parameterization, and debugging
- Practical outcome:
	- A working simulated robot in RViz with basic motion planning and trajectory execution
- Details & lesson material: [Week 1: ROS 2 and MoveIt Foundation](ros_basics/2_moveit_quickstart.md)

## Week 2 — Isaac Sim and Robot Modeling Pipeline
- Topics:
	- Isaac Sim scene graph and robot asset onboarding
	- Physics engine concepts and articulation (links/joints)
	- Sensor simulation and ROS 2 bridge integration
	- URDF structure, collision geometry, and inertial parameters
	- Converting URDF to USD / importing robot into Isaac Sim
- Practical outcome:
	- URDF → Isaac Sim → physics-enabled robot → ROS 2 → MoveIt integration
- Details & lesson material: [Week 2: MoveIt Setup & Modeling](ros_basics/moveit-setup.md)

## Weeks 3–4 — Humanoid Loco-Manipulation PoC
- Track A — Locomotion:
	- Integrate an existing humanoid locomotion policy
	- ROS 2 position-command (or velocity) control for base
	- Waypoint-based walking, stabilization primitives
	- Sequencing: walk-to-station then handoff to arm
- Track B — Manipulation:
	- MoveIt-based arm manipulation (Franka / UR10 examples)
	- Train / integrate pick policy for cube grasping
	- Implement pick-place and handover workflow
- Practical outcome:
	- Humanoid walks to a workstation and performs a single pick task using its manipulator

## Week 5 — Virtual Commissioning & Machine Vision Workflow
- Topics:
	- Industrial workcell digital twin creation (robot, conveyor, fixtures)
	- Robot, conveyor and sensor setup in simulator
	- Camera simulation and ROS 2 image transport
	- OpenCV-based detection, segmentation and messaging to planners
- Practical outcome:
	- Combined industrial cell commissioning with a vision-guided robotic PoC

## Week 6 — Integrated Capstone Project
- Final deliverable integrates:
	- Digital twin onboarding
	- Humanoid or industrial robot simulation
	- Motion planning and execution pipelines
	- Machine-vision trigger to start tasks
	- Workstation end-to-end execution and best-practice review

---

If you want, I can:
- produce printable one-page instructor notes for each week
- create slide decks for Week 1 and Week 2
- add checklists and pre-class setup commands for students

Which would you like next?
