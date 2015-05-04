#ifndef REDSTRAIN_MOD_IO_STREAMCONFIG_HPP
#define REDSTRAIN_MOD_IO_STREAMCONFIG_HPP

namespace redengine {
namespace io {

	enum StreamConfigSpecialization {
		COMPOUND_CONFIG,
		CONFIG_COMPONENT
	};

	struct StreamConfig0 {

		template<StreamConfigSpecialization, typename>
		struct ConfigTemplate {

			struct Config {};

		};

	};

	template<typename FirstConfigT>
	struct StreamConfig1 {

		template<StreamConfigSpecialization, typename>
		struct ConfigTemplate;

		template<typename ComponentT>
		struct ConfigTemplate<COMPOUND_CONFIG, ComponentT> {

			typedef FirstConfigT Config;

		};

		typedef typename ConfigTemplate<COMPOUND_CONFIG, void>::Config Compound;

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, FirstConfigT> {

			static inline FirstConfigT& getConfig(Compound& config) {
				return config;
			}

			static inline const FirstConfigT& getConfig(const Compound& config) {
				return config;
			}

		};

	};

	template<typename FirstConfigT, typename SecondConfigT>
	struct StreamConfig2 {

		template<StreamConfigSpecialization, typename>
		struct ConfigTemplate;

		template<typename ComponentT>
		struct ConfigTemplate<COMPOUND_CONFIG, ComponentT> {

			struct Config {

				FirstConfigT firstConfig;
				SecondConfigT secondConfig;

				Config() {}
				Config(const Config& config)
						: firstConfig(config.firstConfig), secondConfig(config.secondConfig) {}

			};

		};

		typedef typename ConfigTemplate<COMPOUND_CONFIG, void>::Config Compound;

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, FirstConfigT> {

			static inline FirstConfigT& getConfig(Compound& config) {
				return config.firstConfig;
			}

			static inline const FirstConfigT& getConfig(const Compound& config) {
				return config.firstConfig;
			}

		};

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, SecondConfigT> {

			static inline SecondConfigT& getConfig(Compound& config) {
				return config.secondConfig;
			}

			static inline const SecondConfigT& getConfig(const Compound& config) {
				return config.secondConfig;
			}

		};

	};

	template<typename FirstConfigT, typename SecondConfigT, typename ThirdConfigT>
	struct StreamConfig3 {

		template<StreamConfigSpecialization, typename>
		struct ConfigTemplate;

		template<typename ComponentT>
		struct ConfigTemplate<COMPOUND_CONFIG, ComponentT> {

			struct Config {

				FirstConfigT firstConfig;
				SecondConfigT secondConfig;
				ThirdConfigT thirdConfig;

				Config() {}
				Config(const Config& config)
						: firstConfig(config.firstConfig), secondConfig(config.secondConfig),
						thirdConfig(config.thirdConfig) {}

			};

		};

		typedef typename ConfigTemplate<COMPOUND_CONFIG, void>::Config Compound;

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, FirstConfigT> {

			static inline FirstConfigT& getConfig(Compound& config) {
				return config.firstConfig;
			}

			static inline const FirstConfigT& getConfig(const Compound& config) {
				return config.firstConfig;
			}

		};

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, SecondConfigT> {

			static inline SecondConfigT& getConfig(Compound& config) {
				return config.secondConfig;
			}

			static inline const SecondConfigT& getConfig(const Compound& config) {
				return config.secondConfig;
			}

		};

		template<StreamConfigSpecialization Specialization>
		struct ConfigTemplate<Specialization, ThirdConfigT> {

			static inline ThirdConfigT& getConfig(Compound& config) {
				return config.thirdConfig;
			}

			static inline const ThirdConfigT& getConfig(const Compound& config) {
				return config.thirdConfig;
			}

		};

	};

	template<typename ConfigT>
	class StreamConfigurator {

	  public:
		virtual void reconfigStream(ConfigT&) const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_IO_STREAMCONFIG_HPP */
