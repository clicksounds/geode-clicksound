import shutil
import os
shutil.rmtree("resources/AllClicks/MultipleClick")
shutil.copytree(".github/FileNamer/Output", "resources/AllClicks/MultipleClick")
shutil.rmtree(".github/FileNamer/Output")
shutil.rmtree(".github/FileNamer/Update")