extends Node3D

@onready var skeleton := $Armature/Skeleton3D

func _physics_process(delta: float) -> void:
	var new_basis := basis.rotated(Vector3.FORWARD, TAU * (Time.get_ticks_msec() / 2000.0))
	var quat := Quaternion(new_basis)
	skeleton.set_bone_pose_rotation(0, quat)