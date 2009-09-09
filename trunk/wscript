VERSION = '0.1' 
APPNAME = 'libtimefilter'
srcdir = '.' 
blddir = 'build' 

def configure(conf):  
    conf.check_tool('compiler_cc') 
    conf.check_cfg(atleast_pkgconfig_version='0.0.0')
    conf.check_cfg(package='jack', atleast_version='0.109')
    conf.check_cfg(package='jack', args='--cflags --libs')
    conf.find_program('octave')
    conf.check_tool("misc")
    conf.env['CCFLAGS'] = '-Wall'
    conf.env['LINKFLAGS'] = '-lm'

def set_options(opt):  
    opt.tool_options('compiler_cc') 
    opt.add_option('--measure', action='store_true', default=False, 
        help='Measure after building')


def build(bld):
    bld.add_subdirs('src')
    bld.add_subdirs('tests')

    obj = bld.new_task_gen(
	            features = 'subst',
	            source = 'timefilter.pc.in')
    obj.env.table.update({'PKG_LIBS': '-ltimefilter -lm', 'VERSION': VERSION})
    bld.install_files('${PREFIX}/lib/pkgconfig', 'timefilter.pc')



def shutdown():
    cmd = "./measure"

    import os, Options
    if Options.options.measure:
        os.system(cmd)
