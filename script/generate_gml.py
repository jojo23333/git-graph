import re

P = re.compile(r"'.+?'")

mapper = dict()


def write_node(node,id,gml):
    gml.write("  node [\n")
    gml.write("    id "+str(id)+"\n")
    gml.write("    label \""+str(node)+"\"\n")
    gml.write("  ]\n")

def write_edge(source,target,gml):
    gml.write("  edge [\n")
    gml.write("    source "+str(source)+"\n")
    gml.write("    target "+str(target)+"\n")
    gml.write("  ]\n")


def generate_gml(name):
    gml = open(name,"w");
    graph_data = []
    with open("backup","r") as data:
        graph_data  = data.readlines()
    
    gml.write("graph [\n  directed 1")

    user = []
    for node in graph_data[::3]:
        node = node.strip('\n')
        mapper[node] = len(mapper)+1
        user.append(node)
        write_node(node,mapper[node],gml)


    for i,adjs in enumerate(graph_data[1::3]):
        source = user[i]
        for target in P.findall(adjs):
            target = target[1:-2]
            if target in mapper:
                write_edge(mapper[source],mapper[target],gml)

    gml.write("]\n")

generate_gml("git.gml")