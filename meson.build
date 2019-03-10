project('Chuckle', ['cpp', 'c'], default_options : ['cpp_std=c++14'])

cc = meson.get_compiler('cpp')

chuckleCoreProj = subproject('ChuckleCore')
stickLuaSolProj = subproject('StickLuaSol')
crunchLuaSolProj = subproject('CrunchLuaSol')
lukeLuaSolProj = subproject('LukeLuaSol')
paperLuaSolProj = subproject('PaperLuaSol')
dabLuaSolProj = subproject('DabLuaSol')

deps = [chuckleCoreProj.get_variable('chuckleCoreDep'), 
        stickLuaSolProj.get_variable('stickLuaDep'),
        crunchLuaSolProj.get_variable('crunchLuaDep'),
        lukeLuaSolProj.get_variable('lukeLuaDep'),
        paperLuaSolProj.get_variable('paperLuaDep'),
        dabLuaSolProj.get_variable('dabLuaDep')]

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

incDirs = include_directories('.', 'Chuckle/Libs', 'Chuckle/Libs/whereami')

if host_machine.system() == 'linux'
    deps += cc.find_library('dl', required : true)
endif

name = 'chuckle'
bindir = join_paths(get_option('installDir'), 'bin')
chuckle = executable(name, ['Chuckle/Chuckle.cpp', 
    'Chuckle/ChuckleLuaBindings.cpp',
    'Chuckle/Libs/whereami/whereami.c'],
    dependencies: deps, 
    cpp_args:'-ftemplate-depth=10000', 
    include_directories: incDirs,
    install: true,
    install_dir: bindir)

install_subdir('Scripts', install_dir: get_option('installDir'))

#meson symlink workaround, this kind of sucks as it won't be automatically removed on uninstall
meson.add_install_script('ln', '-s', join_paths(bindir, name), join_paths('/usr/local/bin', name))
# custom_target('chuckleSymlink',
#   command : ['ln', '-s', join_paths(bindir, name), join_paths('/usr/local/bin', name)],
#   install: true,
#   install_dir: '/usr/local/bin')
# r = run_command('ln', '-s', join_paths(bindir, name), join_paths('/usr/local/bin', name))
# if r.returncode() != 0
#   warning('Could not create symbolic link for chuckle in /usr/local/bin')
# endif
