/**
 *  @author Koen Wolters <koen.wolters@cern.ch>
 *  @author Daniel Hynds <daniel.hynds@cern.ch>
 */

#ifndef ALLPIX_MODULE_MANAGER_H
#define ALLPIX_MODULE_MANAGER_H

#include <list>
#include <map>
#include <memory>
#include <queue>

#include "../config/Configuration.hpp"
#include "Module.hpp"
#include "core/config/Configuration.hpp"

namespace allpix {

    class ConfigManager;
    class Messenger;
    class GeometryManager;

    class ModuleManager {
    public:
        // Constructor and destructors
        ModuleManager();
        virtual ~ModuleManager();

        // Disallow copy
        ModuleManager(const ModuleManager&) = delete;
        ModuleManager& operator=(const ModuleManager&) = delete;

        // Load modules
        virtual void load(Messenger* messenger, ConfigManager* conf_manager, GeometryManager* geo_manager);

        // Initialize modules (pre-run)
        virtual void init();

        // Run modules
        virtual void run();

        // Finalize modules (post-run)
        virtual void finalize();

    protected:
        using ModuleList = std::list<std::unique_ptr<Module>>;
        using IdentifierToModuleMap = std::map<ModuleIdentifier, ModuleList::iterator>;
        using ModuleToIdentifierMap = std::map<Module*, ModuleIdentifier>;

        // get module identifier
        ModuleIdentifier get_identifier_from_module(Module*);

        // Modules and identifiers converters
        ModuleList modules_;
        IdentifierToModuleMap id_to_module_;
        ModuleToIdentifierMap module_to_id_;
        Messenger* messenger_;
        ConfigManager* conf_manager_;
        GeometryManager* geo_manager_;

        // global allpix configuration
        Configuration global_config_;
        
        // list of loaded libraries
        std::map<std::string, void*> loadedLibraries_;
        
    private:
        // Create modules from the loaded library
        std::vector<std::pair<ModuleIdentifier, Module*>> createModules(Configuration, void*);
        std::vector<std::pair<ModuleIdentifier, Module*>> createModulesPerDetector(Configuration, void*);
        void check_module_detector(const std::string&, Module*, const Detector*);

    };
} // namespace allpix

#endif /* ALLPIX_MODULE_MANAGER_H */
