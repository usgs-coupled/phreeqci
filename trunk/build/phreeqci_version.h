#define PHREEQCI_VER_MAJOR      2
#define PHREEQCI_VER_MINOR     11
#define PHREEQCI_VER_PATCH      0  
#define PHREEQCI_VER_REVISION   0

#define RELEASE_DATE           "@RELEASE_DATE@"

#define APR_STRINGIFY(n) APR_STRINGIFY_HELPER(n)
#define APR_STRINGIFY_HELPER(n) #n

/** Version number */
#define PHREEQCI_VER_NUM       APR_STRINGIFY(PHREEQCI_VER_MAJOR) \
                           "." APR_STRINGIFY(PHREEQCI_VER_MINOR) \
                           "." APR_STRINGIFY(PHREEQCI_VER_PATCH) \
                           "." APR_STRINGIFY(PHREEQCI_VER_REVISION)



#define PRODUCT_NAME       "PHREEQCI" \
                       "-" APR_STRINGIFY(PHREEQCI_VER_MAJOR) \
                       "." APR_STRINGIFY(PHREEQCI_VER_MINOR)


#define PHREEQCI_VER_NUM_REG        APR_STRINGIFY(PHREEQCI_VER_MAJOR) \
                                "." APR_STRINGIFY(PHREEQCI_VER_MINOR)



#define ABOUT_TITLE          "PHREEQC Interactive"
#define ABOUT_VERSION        "Version @VERSION_LONG@"
#define ABOUT_RELEASE_DATE   "Released: @RELEASE_DATE@"
#define ABOUT_IMPLEMENTS     "Implements PHREEQC @VER_UC@"
