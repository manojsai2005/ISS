import filecmp
import os
import shutil
import zipfile

SUB_DIR = "all_subs/"
print("hello")
# Compile the "good code"
os.system("g++ good_code.cpp -o good_code")
GOOD_EXE = os.popen("pwd").read().strip() + "/good_code"
funcs = ["add", "mul", "smul", "trans", "det", "all"]
LOG_DIR = os.popen("pwd").read().strip() + "/logs/"

# For each zip file in the submission directory, extract it into a folder called "scratch"
if os.path.exists("logs"):
    shutil.rmtree("logs")
os.mkdir("logs")
cnt = 0
with open("results.csv", "w") as f:
    f.write(
        "Roll,Part A - add,Part A - mult,Part A - smul,Part A - trans,Part A - det,Part A - combined, Part A - total,Part B - add,Part B - mult,Part B - smul,Part B - trans,Part B - det,Part B - combined, Part B - total,Part C - total,Final Total\n")
    for file in os.listdir(SUB_DIR):
        if file.endswith(".zip"):
            cnt += 1
            A_res = [True, True, True, True, True, True]
            B_res = [True, True, True, True, True, True]
            res = True
            log_name = LOG_DIR + file[:10] + ".log"
            with zipfile.ZipFile(SUB_DIR + file, 'r') as zip_ref:
                shutil.rmtree("scratch", ignore_errors=True)
                zip_ref.extractall("scratch")
                # cd into scratch and execute the command 'gcc */*.c -o main'
                os.chdir("scratch")
                shutil.rmtree("./__MACOSX", ignore_errors=True)
                shutil.rmtree("./*/__MACOSX", ignore_errors=True)
                os.system("gcc $(find -type f -name \"*.c\") -lm -o main >> " + log_name + " 2>&1")

                if not os.path.isfile("main"):
                    print("Compilation failed for " + file)
                    with open(log_name, "a") as log:
                        log.write("Compilation failed\n")
                    f.write(file[:10] + ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n")
                    os.chdir("../")
                    shutil.rmtree("scratch")
                    continue

                os.mkdir("build")
                os.chdir("build")
                os.system("cp ../../tests . -r")
                os.system("cp " + GOOD_EXE + " .")
                os.system("cp ../main .")
                # Perform part C
                # For each file ending with ".in" in the current directory, execute both programs
                for test_file in os.listdir("./tests/C"):
                    if test_file.endswith(".in"):
                        if not res:
                            break
                        tc = "./tests/C/" + test_file
                        if not os.path.exists("outs"):
                            os.mkdir("outs")
                        if os.system(GOOD_EXE + " < " + tc + " > good.out"):
                            print("FATAL ERROR")
                            exit(1)
                        shutil.rmtree("outs")
                        os.mkdir("outs")
                        os.system("timeout 5 ./main < " + tc + " > bad.out 2>>" + log_name)
                        shutil.rmtree("outs")
                        # Compare the output of both programs
                        os.system("grep \"LOG::\" good.out > cmp.good")
                        os.system("grep \"LOG::\" bad.out > cmp.bad")
                        if not filecmp.cmp("cmp.good", "cmp.bad"):
                            print("Test case " + test_file + " failed for " + file)
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " failed\n")
                            res = False
                        else:
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " passed\n")
                        os.system("rm good.out bad.out")

                # Perform part A
                # For each file ending with ".in" in the current directory, execute both programs
                for test_file in os.listdir("./tests/A"):
                    if test_file.endswith(".in"):
                        proceed = True
                        for j in range(len(funcs)):
                            if test_file.startswith(funcs[j]) and not A_res[j]:
                                proceed = False
                        if not proceed:
                            continue

                        tc = "./tests/A/" + test_file
                        if os.system(GOOD_EXE + " < " + tc + " > good.out"):
                            print("FATAL ERROR")
                            exit(1)
                        os.system("timeout 5 ./main < " + tc + " > bad.out 2>>" + log_name)
                        # Compare the output of both programs
                        if not filecmp.cmp("good.out", "bad.out"):
                            print("Test case " + test_file + " failed for " + file)
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " failed\n")
                            for j in range(len(funcs)):
                                if test_file.startswith(funcs[j]):
                                    A_res[j] = False
                                    break
                        else:
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " passed\n")

                        os.system("rm good.out bad.out")

                # Perform part B
                # For each file ending with ".in" in the current directory, execute both programs
                for test_file in os.listdir("./tests/B"):
                    if test_file.endswith(".in"):
                        proceed = True
                        for j in range(len(funcs)):
                            if test_file.startswith(funcs[j]) and not B_res[j]:
                                proceed = False
                        if not proceed:
                            continue
                        shutil.rmtree("outs", ignore_errors=True)
                        shutil.rmtree("good_outs", ignore_errors=True)
                        os.mkdir("outs")
                        tc = "./tests/B/" + test_file
                        if os.system(GOOD_EXE + " < " + tc + " > good.out"):
                            print("FATAL ERROR")
                            exit(1)
                        os.system("mv outs good_outs")
                        os.mkdir("outs")
                        os.system("timeout 5 ./main < " + tc + " > bad.out 2>>" + log_name)
                        # Compare the output of both programs
                        if not filecmp.cmp("good.out", "bad.out"):
                            print("Test case " + test_file + " failed for " + file)
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " failed\n")
                            for j in range(len(funcs)):
                                if test_file.startswith(funcs[j]):
                                    B_res[j] = False
                            shutil.rmtree("outs")
                            shutil.rmtree("good_outs")
                            os.system("rm good.out bad.out")
                            continue
                        else:
                            with open(log_name, "a") as log:
                                log.write("Test case " + test_file + " passed\n")

                        # Compare each file present in directories "outs" and "good_outs"
                        for out_file in os.listdir("good_outs"):
                            if not os.path.isfile("outs/" + out_file) or not filecmp.cmp("outs/" + out_file,
                                                                                         "good_outs/" + out_file):
                                print("Test case " + test_file + " failed for " + file)
                                with open(log_name, "a") as log:
                                    log.write("Test case " + test_file + " failed\n")
                                for j in range(len(funcs)):
                                    if test_file.startswith(funcs[j]):
                                        B_res[j] = False
                                        break
                                break
                            else:
                                with open(log_name, "a") as log:
                                    log.write("Test case " + test_file + " passed\n")

                        shutil.rmtree("outs")
                        shutil.rmtree("good_outs")
                        os.system("rm good.out bad.out")

            os.chdir("../../")
            shutil.rmtree("scratch")
            print("Tested " + file + " successfully")
            part_A, part_B, part_C = 0, 0, 0
            to_write = file[:10] + ","
            for i in A_res:
                if i:
                    part_A += 5
                    to_write += "5,"
                else:
                    to_write += "0,"
            to_write += str(part_A) + ","

            for i in B_res:
                if i:
                    part_B += 5
                    to_write += "5,"
                else:
                    to_write += "0,"
            to_write += str(part_B) + ","

            if res:
                part_C += 20
                to_write += "20,"
            else:
                to_write += "0,"
            to_write += str(part_A + part_B + part_C)
            f.write(to_write + "\n")

            print("Part A: " + str(A_res), part_A)
            print("Part B: " + str(B_res), part_B)
            print("Part C: " + str(res), part_C)

            print("Done with ", cnt)
