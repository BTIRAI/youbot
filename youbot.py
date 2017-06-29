import sys
sys.path.insert(0, 'bt/')
from SequenceNode import SequenceNode
from FallbackNode import FallbackNode
from NewDraw import new_draw_tree

from ActionTest import ActionTest
from ConditionTest import ConditionTest

from BTSearchActionNodes import *
from BTSearchConditionNodes import *
from vrep_api import *



def Experiment1():
    vrep = vrep_api()
    green_cube_id = vrep.get_id(b'greenRectangle1')
    goal_id = vrep.get_id(b'goalRegion')




    move_to_cube_subtree = FallbackNode('Fallback')

    move_to_blue_cube = MoveCloseTo('MoveToGreenCube', green_cube_id, vrep)
    is_robot_close_to_green_cube = IsRobotCloseTo('IsRobotClosetoGreenCube', green_cube_id, vrep)

    move_to_cube_subtree.AddChild(is_robot_close_to_green_cube)
    move_to_cube_subtree.AddChild(move_to_blue_cube)

    grasp_cube_subtree = FallbackNode('Fallback')

    grasp_cube_sequence = SequenceNode('GraspCubeSequence')

    grasp_green_cube = GraspObject('GraspGreenCube', green_cube_id, vrep)
    is_green_cube_grasped = IsObjectGrasped('IsGreenCubeGrasped', green_cube_id, vrep)

    grasp_cube_sequence.AddChild(move_to_cube_subtree)
    grasp_cube_sequence.AddChild(grasp_green_cube)

    grasp_cube_subtree.AddChild(is_green_cube_grasped)
    grasp_cube_subtree.AddChild(grasp_cube_sequence)

    move_to_goal_subtree = FallbackNode('Fallback')

    move_to_goal = MoveCloseTo('MoveToGoal', goal_id, vrep)
    is_robot_close_to_goal = IsRobotCloseTo('IsRobotClosetoGoal', goal_id, vrep)

    move_to_goal_subtree.AddChild(is_robot_close_to_goal)
    move_to_goal_subtree.AddChild(move_to_goal)



    place_blue_cube_sequence = SequenceNode('PlaceGreenCube')

    place_blue_cube = DropObject('DropGreenCube', vrep)

    place_blue_cube_sequence.AddChild(grasp_cube_subtree)
    place_blue_cube_sequence.AddChild(move_to_goal_subtree)
    place_blue_cube_sequence.AddChild(place_blue_cube)

    is_green_cube_at = IsObjectAt('IsGreenAtGoal', green_cube_id, goal_id, vrep)



    root = FallbackNode('root')

    root.AddChild(is_green_cube_at)
    root.AddChild(place_blue_cube_sequence)

    draw_thread = threading.Thread(target=new_draw_tree, args=(root,))
    draw_thread.start()
    while True:
        root.Execute(None)
        time.sleep(1)



if __name__ == "__main__":
    Experiment1()