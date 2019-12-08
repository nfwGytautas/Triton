import argparse

parser = argparse.ArgumentParser("Triton shader compiler")
parser.add_argument("shader", help="The shader file name", type=str)
parser.add_argument("out", help="The compiled file name", type=str)
args = parser.parse_args()

tcn_content = ""
shader_content = ""

dependencies = []

with open(args.shader, "r") as fin:
    for line in fin:
        index = line.find("#include")
        if index != -1:
            d_name = line[len("#include"):].strip().replace("\"", "")
            dependencies.append(d_name)
            print("Found '{}'".format(d_name))
        else:
            shader_content += line


for depend in dependencies:
    with open(depend) as din:
        dependency_content = ""
        for line in din:
            index = line.find("#include")
            if index != -1:
                d_name = line[len("#include"):].strip().replace("\"", "")

                if d_name not in dependencies:
                    dependencies.append(d_name)
                    print("Found nested '{}'".format(d_name))
            else:
                dependency_content += line

        tcn_content += dependency_content + "\n"

tcn_content += shader_content

with open(args.out, "w") as fout:
    fout.write(tcn_content)
