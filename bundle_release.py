# libraries
import os, shutil
from datetime import date

# config
config = {
    "make_setup": True,               # set True if you want to make setup version
    "make_portable": True,            # set True if you want to make portable version
    "make_ota_package": False,        # set True if you want to make OTA package
    "ota_enabled": False,             # set True if ota enabled
    "repo_path": "",                  # leave it blank if this script located in repo folder
    "version": "",                    # leave it blank to fill with version from SourceFiles/core/version.h and script runtime date
    "tgversion": "",                  # leave it blank to set it automatically
    "intversion": 0,                  # leave it 0 to set automatically
    "exe_filename": "rabbitGram.exe", # set executable name here
}

# functions
def log(text, level):
    print(f"#{" " * level}{text}")

def set_repo_path():
    log("Setting repo path...", 1)
    config["repo_path"] = os.getcwd()
    log("Repo path was not set in config", 2)
    log(f"Set repo path to: {config["repo_path"]}", 2)

def set_version():
    log("Setting version...", 1)
    with open(f"{config["repo_path"]}/Telegram/SourceFiles/core/version.h", "r") as version_file:
        version_code = version_file.readlines()[25]
        config["version"] += version_code.replace('constexpr auto AppVersionStr = "', '').replace('";', date.today().strftime("-%d%m%Y")).replace('\n', '')
        config["tgversion"] += version_code.replace('constexpr auto AppVersionStr = "', '').replace('";', '').replace('\n', '')
    with open(config["repo_path"] + "/Telegram/SourceFiles/core/version.h", "r") as version_file:
        int_version_code = version_file.readlines()[24]
        config["intversion"] += int(int_version_code.replace("constexpr auto AppVersion = ", '').replace(';', '').replace('\n', ''))

def set_iss():
    log("Updating iss file...", 1)
    iss_file = open(f"{config["repo_path"]}/Telegram/build/setup.iss", "r", encoding="utf-8")
    iss_file_data = iss_file.readlines()
    iss_file_data[3] = f'#define MyAppVersion "{config["tgversion"]}"\n'
    iss_file_data[10] = f'#define MyAppVersionFull "{config["version"]}"\n'
    iss_file.close()
    iss_file = open(f"{config["repo_path"]}/Telegram/build/setup.iss", "w", encoding="utf-8")
    iss_file.writelines(iss_file_data)

def check_files():
    log("# Renaming files...", 1)

    if not os.path.exists(os.path.join(f"{config["repo_path"]}/out/Release/{config["exe_filename"]}")):
        log(f"{config["exe_filename"]} does not exist too, halt...", 2)
        exit()

def run_iss_build():
    log("Running iscc build... If error occurs, check if iscc.exe path added to PATH", 1)
    os.system(f"iscc {config["repo_path"]}/Telegram/build/setup.iss")

def make_portable():
    log("Making portable version", 1)
    log("Creating 'portable' folder", 2)
    try:
        os.mkdir(os.path.join(f"{config["repo_path"]}/out/Release/portable"))
        log("Created 'portable' folder", 3)
    except:
        log("Folder 'portable' already exists", 3)
    log("Copying portable files", 2)
    try:
        shutil.copyfile(f"{config["repo_path"]}/out/Release/{config["exe_filename"]}", f"{config["repo_path"]}/out/Release/portable/{config["exe_filename"]}")
        if config["ota_enabled"]:
            shutil.copyfile(f"{config["repo_path"]}/out/Release/Updater.exe", f"{config["repo_path"]}/out/Release/portable/Updater.exe")
        shutil.copytree(f"{config["repo_path"]}/out/Release/modules", f"{config["repo_path"]}/out/Release/portable/modules")
        log("Files copied to 'portable' folder", 3)
    except:
        log("Files already exist", 3)
    log("Making archive...", 2)
    shutil.make_archive(f"{config["repo_path"]}/out/Release/releases/rtgdrelease-{config["version"]}/rtgdportable-x64.{config["version"]}", 'zip', f"{config["repo_path"]}/out/Release/portable")

def make_ota_package():
    log("Making OTA package", 1)
    first_loc = os.getcwd()
    os.chdir(f"{config["repo_path"]}/out/Release")
    cmd = f"Packer.exe -path {config['exe_filename']} -version {config['intversion']} -target win64"
    os.system(cmd)
    os.chdir(first_loc)
    shutil.copyfile(f"{config["repo_path"]}/out/Release/tx64upd{config["intversion"]}", f"{config["repo_path"]}/out/Release/releases/rtgdrelease-{config["version"]}/tx64upd{config["intversion"]}")

if config["repo_path"] == "":
    set_repo_path()
if config["version"] == "":
    set_version()
check_files()
if config["make_setup"]:
    set_iss()
    run_iss_build()
if config["make_portable"]:
    make_portable()
if config["make_ota_package"]:
    make_ota_package()

log("All done!", 1)
