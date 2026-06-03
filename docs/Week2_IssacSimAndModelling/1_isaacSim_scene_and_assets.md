# Isaac Sim — Scene & Assets Quickstart

> Source: Based on the NVIDIA Isaac Sim Quickstart: https://docs.isaacsim.omniverse.nvidia.com/latest/introduction/quickstart_isaacsim.html
Overview
========

1. Launch Isaac Sim

- Preferred: Open Omniverse Launcher → Isaac Sim → Launch.
- If using a kit install, run the Isaac Sim executable from the install folder (consult NVIDIA Quickstart for platform-specific commands).

2. Create a new stage

- File → New Stage (or use the New Stage button). Choose a simple default path in the local project folder.
- Save early: File → Save As → `<project_root>/scenes/quickstart_stage.usd`

3. Add scene primitives

- Open the Content Browser → Primitives → Drag in a `Cube`, `Plane`, and `Sphere`.
- Position the primitives using the Translate gizmo; rotate and scale as needed.
- Add a `Distant Light` or `RectLight` from the Create menu.

4. Inspect stage hierarchy and properties

- Open the Stage Tree/Prim Browser and click each prim to show its properties.
- Use the Property panel to inspect transform, materials, and custom attributes.

5. Import external assets

- From the Content Browser, use Import or `File → Import` to bring in a USD file.
- Alternatively, follow the URDF-to-USD guide in the Isaac Sim docs to convert robot URDFs into USD assets for simulation.

6. Enable physics for selected prims

- Select a prim → Physics → Add Rigid Body (or right-click → Add Physics).
- Set mass, collision shape, and solver properties in the Physics panel.

7. Run the simulation

- Use the Play controls (Timeline / Play button) to run the simulation and observe dynamics.

Hands-on exercises (detailed)
-----------------------------


