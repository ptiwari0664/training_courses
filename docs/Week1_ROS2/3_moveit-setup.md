# MoveIt Setup Assistant — Configure Moveit, Kinematics and Planners 

> Setup Assistance:
https://moveit.picknik.ai/main/doc/examples/setup_assistant/setup_assistant_tutorial.html

> Perception Pipeline
https://moveit.picknik.ai/main/doc/examples/perception_pipeline/perception_pipeline_tutorial.html

## Overview
- MoveIt Setup Assistant is a GUI for configuring a robot for MoveIt.
- Primary output: SRDF and supporting MoveIt configuration files.
- Requires a valid robot URDF file.
- Best for teaching: load a sample robot, generate config, inspect files.

## Getting Started
- Install MoveIt and ROS2 first.
- Use `moveit_resources_panda_description` for the example panda robot.
- Ensure your workspace contains the robot URDF.
- Recommended path: `~/ws_moveit2/src/moveit_resources/panda_description/urdf/panda.urdf`.
- If an old config package exists, delete it before starting.

## Step 1: Start
- Run: `ros2 launch moveit_setup_assistant setup_assistant.launch.py`
- Choose: Create New MoveIt Configuration Package.
- Click Browse and select the robot URDF file.
- Click Load Files.
- Wait for the Setup Assistant to parse the URDF.
- Confirm the robot model loads successfully.

## Step 2: Generate Self-Collision Matrix
- Open the Self-Collisions pane.
- Set self-collision sampling density (default 10,000).
- Click Generate Collision Matrix.
- Wait for the computation to finish.
- Review the collision matrix table.
- Optionally adjust checkmarks manually.
- Teaching point: disable always-safe collisions to speed planning.

## Step 3: Add Virtual Joints
- Open the Virtual Joints pane.
- Click Add Virtual Joint.
- Set joint name, e.g. `virtual_joint`.
- Set parent frame to `world`.
- Set child link to the robot base link.
- Set joint type to `fixed` for a fixed-base robot.
- Click Save.
- Teaching point: virtual joints attach the robot model to the world frame.

## Step 4: Add Planning Groups
- Open the Planning Groups pane.
- Click Add Group.
- Add the arm group first.
- Enter group name, e.g. `panda_arm`.
- Choose a kinematics solver (default KDL or custom IK plugin).
- Keep Kin. Search Resolution and Kin. Search Timeout default.
- Click Add Joints.
- Select the joint chain from `virtual_joint` to end joint.
- Click the > button to add selected joints.
- Click Save.
- Add the hand/gripper group next.
- Click Add Group.
- Enter group name, e.g. `hand`.
- Set kinematic solver to None for a non-chain end effector.
- Click Add Links.
- Select gripper links and add them.
- Click Save.
- Teaching point: planning groups define what MoveIt plans for.

## Step 5: Add Robot Poses
- Open the Robot Poses pane.
- Click Add Pose.
- Give the pose a descriptive name, e.g. `ready` or `home`.
- Move joints in the preview to set the pose.
- Save the pose.
- Repeat for a gripper open pose and close pose if needed.
- Teaching point: saved poses help with initial and demo configurations.

## Step 6: Label End Effectors
- Open the End Effectors pane.
- Click Add End Effector.
- Choose the gripper group as the end effector group.
- Select the parent link of the end effector.
- Leave parent group blank if not needed.
- Save the end effector.
- Teaching point: end effectors are used for attach/detach and grasping.

## Step 7: Add Passive Joints
- Open the Passive Joints pane.
- Add any passive joints if your robot has them.
- Skip this step if there are no passive joints.
- Teaching point: passive joints are unactuated and should not be planned.

## Step 8: ros2_control URDF Modification
- Open the ros2_control URDF Modification pane.
- Add command interfaces for each joint.
- Add state interfaces for each joint.
- Default: position command, position and velocity state.
- Click Add Interfaces.
- Teaching point: this prepares the URDF for ros2_control integration.

## Step 9: ROS 2 Controllers
- Open the ROS 2 Controllers pane.
- Click Add Controller.
- Enter a controller name, e.g. `panda_arm_controller`.
- Choose controller type, e.g. `joint_trajectory_controller/JointTrajectoryController`.
- Add joints manually or use Add Planning Group Joints.
- Select the relevant planning group.
- Save the controller.
- Repeat for the gripper controller.
- Teaching point: controllers map MoveIt commands to robot actuation.

## Step 10: MoveIt Controllers
- Open the MoveIt Controllers pane.
- Click Add Controller.
- Enter controller name, e.g. `panda_arm_controller`.
- Choose FollowJointTrajectory controller type.
- Add joints from the `panda_arm` planning group.
- Save the controller.
- Add a gripper controller with Gripper Command type.
- Save the controller.
- Teaching point: MoveIt controllers are used by the MoveIt controller manager.

## Step 11: Perception
- Open the Perception pane.
- Choose 3D sensor configuration or select None.
- If using sensors, configure pointcloud parameters.
- Save the perception settings.
- Teaching point: perception settings connect sensor data to the planning scene.

## Step 12: Launch Files
- Open the Launch Files pane.
- Review the list of generated launch files.
- Inspect each launch file description.
- Note default options are usually sufficient.
- Teaching point: generated launch files are ready for demo and robot use.

## Step 13: Add Author Information
- Open the Author Information pane.
- Enter your name.
- Enter your email address.
- Teaching point: metadata is required for ROS package generation.

## Step 14: Generate Configuration Files
- Open the Configuration Files pane.
- Choose a package name, e.g. `panda_moveit_config`.
- Choose a destination folder in your ROS2 workspace `src`.
- Click Generate Package.
- Review the generated files in the output list.
- Teaching point: this is the final export step for the MoveIt config package.

## Build and Run Example
- Run: `colcon build --packages-select panda_moveit_config`
- Source the install space: `source install/setup.bash`
- Launch the demo: `ros2 launch panda_moveit_config demo.launch.py`
- Teaching point: verify the generated config by running RViz with the generated package.

## Notes
- Use bullet points while dictating each step.
- Keep all instructions simple and sequential.
- Refer to PickNik example page for images and detailed screenshots.
- Use ROS2 commands where possible for MoveIt2 students.
