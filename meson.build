project('Chuckle', ['cpp', 'c'], default_options : ['cpp_std=c++14', 'prefix=/opt/Chuckle', 'libdir=lib', 'default_library=static'])

cc = meson.get_compiler('cpp')

chuckleCoreProj = subproject('ChuckleCore')
stickLuaSolProj = subproject('StickLuaSol')
crunchLuaSolProj = subproject('CrunchLuaSol')
lukeLuaSolProj = subproject('LukeLuaSol')
paperLuaSolProj = subproject('PaperLuaSol')
dabLuaSolProj = subproject('DabLuaSol')
picLuaSol = subproject('PicLuaSol')
utf8er = subproject('utf8er')

deps = [chuckleCoreProj.get_variable('chuckleCoreDep'), 
        stickLuaSolProj.get_variable('stickLuaDep'),
        crunchLuaSolProj.get_variable('crunchLuaDep'),
        lukeLuaSolProj.get_variable('lukeLuaDep'),
        paperLuaSolProj.get_variable('paperLuaDep'),
        dabLuaSolProj.get_variable('dabLuaDep'),
        picLuaSol.get_variable('picLuaDep'),
        utf8er.get_variable('utf8erDep')]

# meseon dependency function to find lua is not very consistent accross platforms yet, so
# we need to do a lot of manual lifting for now
foreach name : ['lua', 'lua5.3', 'lua-5.3', 'lua53']
    luaDep = dependency(name, version: '>=5.3', required: false)
    if luaDep.found()
        break
    endif
endforeach

if not luaDep.found()
    error('Lua could not be found!')
endif

deps += luaDep

deps += cc.find_library('Box2D', required : true)

incDirs = include_directories('.', 'Libs')

if host_machine.system() == 'linux'
    deps += cc.find_library('dl', required : true)
endif

name = 'chuckle'
bindir = join_paths(get_option('prefix'), get_option('bindir'))
libdir = join_paths(get_option('prefix'), get_option('libdir'))
chuckle = executable(name, [
    'Chuckle/Box2DLuaBindings.cpp',
    'Chuckle/Chuckle.cpp', 
    'Chuckle/ChuckleLuaBindings.cpp',
    'Chuckle/CrunchLuaSol.cpp',
    'Chuckle/DabLuaSol.cpp',
    'Chuckle/ImGuiLuaBindings.cpp',
    'Chuckle/LukeLuaSol.cpp',
    'Chuckle/PaperLuaSol.cpp',
    'Chuckle/PicLuaSol.cpp'],
    dependencies: deps, 
    cpp_args:'-ftemplate-depth=10000', #for sol lua binding compilation on gcc oO
    include_directories: incDirs,
    install_rpath: libdir,
    install: true)

install_subdir('Scripts', install_dir:'.')
install_subdir('Assets', install_dir:'.')

#meson symlink workaround, this kind of sucks as it won't be automatically removed on uninstall
if get_option('prefix') != '/usr/local'
    meson.add_install_script('ln', '-s', join_paths(bindir, name), join_paths('/usr/local/bin', name))
endif
