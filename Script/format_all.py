import re
import subprocess
import platform
from os import path, walk

DIR_PATH = path.join(path.dirname(__file__))
ALL_FILES_DIR = path.abspath(path.join(DIR_PATH, '../..', 'Client'))
ALL_FILES_ROOT_DIR = path.abspath(path.join(DIR_PATH, '../..'))

CODE_PATH_REGEXPS = [
    r"^Client/Source/.*",
    r"^Client/Plugins/DataAssetExtender/Source/.*",
    r"^Client/Plugins/DataTableExtender/Source/.*",
    r"^Client/Plugins/CreatureFramework/Source/.*",
    r"^Client/Plugins/ATInventory/Source/.*",
    r"^Client/Plugins/BackendRpc/Source/.*",
    r"^Client/Plugins/DaedalicTestAutomationPlugin/Source/.*",
    r"^Client/Plugins/DataAssetExtender/Source/.*",
    r"^Client/Plugins/DataTableExtender/Source/.*",
    r"^Client/Plugins/DataTableSaveBluprintLibrary/Source/.*",
    r"^Client/Plugins/DeviousMarkup/Source/.*",
    r"^Client/Plugins/DeviousUtils/Source/.*",
    r"^Client/Plugins/Diagnostic/Source/.*",
    r"^Client/Plugins/GameplayAttributesUtils/Source/.*",
    r"^Client/Plugins/GameplayDebug/Source/.*",
    r"^Client/Plugins/GameplayModuleUtils/Source/.*",
    r"^Client/Plugins/GitPro/Source/.*",
    r"^Client/Plugins/OverlapSubsystem/Source/.*",
    r"^Client/Plugins/PerformanceTelemetry/Source/.*",
    r"^Client/Plugins/ServerApi/Source/.*",
    r"^Client/Plugins/SmartPlacement/Source/.*",
    r"^Client/Plugins/SoundPrioritySubsystem/Source/.*",
    r"^Client/Plugins/TokenSystem/Source/.*",
    r"^Client/Plugins/SDFGenerator/Source/.*",
    r"^Client/Plugins/Linter/Source/.*"
]

CODE_EXT_REGEXP = r"cpp|h|hpp"


def get_clang_path() -> str:
    bin_name = {
        'Windows':  'clang-format.exe',
        'Linux':    'clang-format',
        'Darwin':   'clang-format',
    }[platform.system()]

    bin_path = path.abspath(path.join(path.dirname(__file__), bin_name))
    return bin_path


def need_to_check_format(file: str) -> bool:
    file_ext = path.splitext(file)[1][1:]
    if not re.match(CODE_EXT_REGEXP, file_ext):
        return False

    file = file.replace('\\', '/')
    match_filter = filter(lambda x: re.match(x, file), CODE_PATH_REGEXPS)
    return any(match_filter)


def run_format(bin_path: str, file: str):
    subprocess.run([bin_path, "-i", file], shell=True, cwd=ALL_FILES_ROOT_DIR)


fmt_path = get_clang_path()
for dir_, _, files in walk(ALL_FILES_DIR):
    rel_dir = path.relpath(dir_, ALL_FILES_ROOT_DIR)

    for file_name in files:
        rel_file = path.join(rel_dir, file_name)
        if need_to_check_format(rel_file):
            run_format(fmt_path, rel_file)
