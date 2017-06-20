#include <L2Fsim/simulation.hpp>
#include <L2Fsim/utils/cfg_reader.hpp>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <libconfig.h++>

using namespace L2Fsim;

/**
 * @brief Method creating an environment and saving it at the specified location
 * @param {const bool &} save; if true, save the scenario for vizualization
 */
void create_environment(const bool &save) {
    /**
     * @brief Parameters; See the used flight zone for description of the parameters
     * @param {double} dt; thermal refreshment rate (s)
     * @param {int} model; thermal model selection
     * 1: Allen
     * 2: Childress
     * 3: Lenschow
     * 4: Geodon
     * 5: Lawrance
     */
    int model = 1;
    double dt = 1.;

    double t_start = -500.;
    double t_limit = 1000.;
    double windx = 0.;
    double windy = 0.;
    double w_star_min = 2.;
    double w_star_max = 4.;
    double zi_min = 1300.;
    double zi_max = 1400.;
    double lifespan_min = 300.;
    double lifespan_max = 600.;
    double x_min = -1500.;
    double x_max = +1500.;
    double y_min = -1500.;
    double y_max = +1500.;
    double z_min = +0.;
    double z_max = +2000.;
    double ksi_min = .3;
    double ksi_max = .7;
    double d_min = 150.;
    unsigned int nbth = 15;

    /// 1. Initialize an empty zone and define its boundaries
    flat_thermal_soaring_zone fz(
        t_start, t_limit,
        windx, windy,
        w_star_min, w_star_max,
        zi_min, zi_max,
        lifespan_min, lifespan_max,
        x_min, x_max,
        y_min, y_max,
        z_min, z_max,
        ksi_min, ksi_max,
        d_min, nbth);
    //flat_thermal_soaring_zone fz_from_file("config/thermal_scenario.csv","config/fz_config.csv");

    /// 2. Create a scenario i.e. create the thermals
    fz.create_scenario(dt,model);
    //fz.print_scenario(); // print the created thermals (optional)

    /// 3 : Save data for visualization (optional)
    if(save) {
        double dx=5., dy=5.; // mesh precision
        double z=500., t=500.;  // altitude and time of the saved updraft field
        fz.save_updraft_values(dx,dy,z,t,"data/updraft_field.dat");
    }

    /// 4. Save the scenario
    fz.save_scenario("config/thermal_scenario.csv");
    fz.save_fz_cfg("config/fz_config.csv");
}

/**
 * @brief Method running a simulation using a configuration file
 * @param {const char *} cfg_path; path to the configuration file
 */
void run_with_config(const char *cfg_path) {
    cfg_reader cfgr;
    libconfig::Config cfg;
    cfg.readFile(cfg_path);

    /** Initialize the simulation */
    simulation mysim;

    /** General settings */
	mysim.st_log_path = cfgr.read_st_log_path(cfg);
	mysim.fz_log_path = cfgr.read_fz_log_path(cfg);
    double Dt=.1, t_lim=1e3, nb_dt=1., t=0.; // default values
    cfgr.read_time_variables(cfg,t_lim,Dt,nb_dt);

    /** Environment */
    mysim.fz = cfgr.read_environment(cfg);

    /** Aircraft */
	mysim.ac = cfgr.read_aircraft(cfg);

    /** Stepper */
	mysim.st = cfgr.read_stepper(cfg,Dt/nb_dt);

    /** Pilot */
	mysim.pl = cfgr.read_pilot(cfg);

	/** Run the simulation */
	bool eos = false;
	mysim.clear_saves();
    while(!(is_greater_than(t,t_lim)) && !eos) {
        std::cout<<t<<std::endl;
        mysim.save();
        mysim.step(t,Dt,eos);
    }

	/** End of simulation */
	std::cout << "Program run successfully" << std::endl;
}

int main() {
    try
    {
        srand(time(NULL));
        create_environment(true);
        run_with_config("config/main_config.cfg");
    }
    catch(const std::exception &e)
    {
        std::cerr<<"[error] In main(): standard exception caught: "<<e.what()<<std::endl;
    }
    catch(...)
    {
        std::cerr<<"[error] In main(): Unknown exception caught"<<std::endl;
    }
    return 0;
}
