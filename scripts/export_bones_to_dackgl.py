import bpy


def write_some_data(context, filepath):
    print("exporting dackgl file for %s" % bpy.data.filepath)
    
    f = open(filepath, 'w')

    for mesh in bpy.context.selected_objects:
        print_mesh(f, mesh.data)
        
    for armature in bpy.data.armatures:
        print_armatures(f, armature)
    
    f.close()

    return {'FINISHED'}

def print_mesh(f, mesh):
    f.write('m %s\n' % mesh.name)
    print_vertices(f, mesh)
    print_faces(f, mesh)
    print_vertex_groups(f, mesh)
    
def print_armatures(f, armature):
    f.write('a %s\n' % armature.name)
    print_bones(f, armature)

def print_vertices(f, mesh):
    f.write('v %d\n' % len(mesh.vertices))
    for (i, vertex) in mesh.vertices.items():
        f.write('%.10f,%.10f,%.10f ' % vertex.co[:])
        f.write('%.10f,%.10f,%.10f\n' % vertex.normal[:])

def print_faces(f, mesh):
    faces = []
    for face in mesh.faces:
        v = face.vertices
        faces.append(v[:3])
        if len(v) == 4: # triangulate if necessary
            faces.append(v[2:] + v[:1])
            
    f.write('f %d\n' % len(faces))
    for face in faces:
        f.write('%d %d %d\n' % face)

def print_vertex_groups(f, mesh):
    f.write('g %d\n' % len(mesh.vertices))
    for (i, vertex) in mesh.vertices.items():
        for group in vertex.groups.values():
            f.write('%d:%.10f ' % (group.group, group.weight))
        f.write('\n')

def print_bones(f, armature):
    f.write('b %d\n' % len(armature.bones))
    for bone in armature.bones:
        f.write('%s ' % bone.name)
        for r in bone.matrix_local:
            for e in r:
                f.write("%.10f " % e)
        f.write("\n")
    


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportSomeData(bpy.types.Operator, ExportHelper):
    '''This appears in the tooltip of the operator and in the generated docs.'''
    bl_idname = "export.bones_to_dackgl"  # this is important since its how bpy.ops.export.some_data is constructed
    bl_label = "Export Bones to Dackgl Game Engine"

    # ExportHelper mixin class uses this
    filename_ext = ".dack"

    filter_glob = StringProperty(default="*.dack", options={'HIDDEN'})

    @classmethod
    def poll(cls, context):
        return context.active_object != None

    def execute(self, context):
        return write_some_data(context, self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportSomeData.bl_idname, text="Dack GL Game Engine (.dack)")


def register():
    bpy.utils.register_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export.bones_to_dackgl('INVOKE_DEFAULT')