Animator
===================
A first attempt at implementing forward and inverse kinematics.

Contents
I.      Directions
II.     Features
III.    Implementation
IV.     Sources

I. Directions
Run with: bin/main static/skeletonN.skel
Press the space bar to switch between forward and inverse kinematics mode.
Press left or right to rotate the skeleton.

    i. Forward Kinematics
    Hold the mouse down on the bone you want to manipulate, then drag the mouse
    up or down.  The joint will rotate along its axis.

    ii. Inverse Kinematicsk
    Click on a bone to add or remove a constrant for it.  Use the mouse to drag
    and drop the constrant.  Right click and drag up or down on the constraint
    to move it in or out of the screen.


II. Features
This system only supports rotational joints, and allows specification of an
arbitrary skeleton in the .skel format, detailed below.  The mouse only
interface allows for easy visual manipulation of the skeleton.

skel format:
Your skeleton must be a rooted tree, where each joint is a node, and the .skel
format represents a preorder traversel of that tree.  For each joint, you must
specify in order: length, rotation axis, initial angle, and the number of direct
children it has.  Check out the examples.


III. Implementation
The skeleton data structure is a tree of joints, each with a rotational axis and
angle.
    
    i. Forward Kinematics
    I use the OpenGL matrix stack to traverse the skeleton tree and draw joints
    in their appropriate locations.

    ii. Inverse Kinematics
    The IK interface uses glm to project the mouse position to coordinates in
    the world with a specified depth.  It provides tools to make matrix
    calculations easy.  The IK minimization algorithm is done using Eigen's
    NonLinearOptimization module.  The Jacobian is calculated by performing a
    first order Taylor approximation.

IV. Sources
My use of Eigen draws heavily from the provided examples.  I discussed with a
friend from home how to write the mouseToObj function for the IK interface, so
that was a joint effort.  Besides this, old StackOverflow questions and
similar sites provided a lot of debugging help.
