/****************************************************************************
 *
 *   Copyright (c) 2020 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file pid_autotune_angular_rate.hpp
 *
 * @author Mathieu Bresciani <mathieu@auterion.com>
 */

#pragma once

#include <drivers/drv_hrt.h>
#include <lib/perf/perf_counter.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/WorkItem.hpp>
#include <systemlib/mavlink_log.h>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionCallback.hpp>
#include <uORB/topics/actuator_controls.h>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/pid_autotune_angular_rate_status.h>
#include <uORB/topics/vehicle_angular_velocity.h>
#include <mathlib/mathlib.h>

#include "system_identification.hpp"
#include "pid_design.hpp"

class PidAutotuneAngularRate : public ModuleBase<PidAutotuneAngularRate>, public ModuleParams,
	public px4::WorkItem
{
public:
	PidAutotuneAngularRate();
	~PidAutotuneAngularRate() override;

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	bool init();

	/** @see ModuleBase::print_status() */
	int print_status() override;

private:
	void Run() override;
	void updateParams() override;

	void reset();

	void checkFilters();

	void updateStateMachine(const matrix::Vector<float, 5> &coeff_var, hrt_abstime now);
	bool areAllSmallerThan(matrix::Vector<float, 5> vect, float threshold);

	const matrix::Vector3f getIdentificationSignal();

	orb_advert_t _mavlink_log_pub{nullptr};	/**< Mavlink log uORB handle */

	uORB::SubscriptionCallbackWorkItem _actuator_controls_sub{this, ORB_ID(actuator_controls_0)};

	uORB::Subscription _parameter_update_sub{ORB_ID(parameter_update)};
	uORB::Subscription _vehicle_angular_velocity_sub{ORB_ID(vehicle_angular_velocity)};
	uORB::Subscription _manual_control_setpoint_sub{ORB_ID(manual_control_setpoint)};
	uORB::Publication<pid_autotune_angular_rate_status_s> _pid_autotune_angular_rate_status_pub{ORB_ID(pid_autotune_angular_rate_status)};

	SystemIdentification _sys_id;

	enum class state {idle, roll, roll_pause, pitch, pitch_pause, yaw, yaw_pause, verification, complete} _state{state::idle};
	hrt_abstime _state_start_time{0};
	uint8_t _steps_counter{0};
	uint8_t _max_steps{5};
	int8_t _signal_sign{0};

	/**
	 * Scale factor applied to the input data to have the same input/output range
	 * When input and output scales are a lot different, some elements of the covariance
	 * matrix will collapse much faster than other ones, creating an ill-conditionned matrix
	 */
	float _input_scale{1.f};

	hrt_abstime _last_run{0};
	hrt_abstime _last_publish{0};

	float _interval_sum{0.f};
	float _interval_count{0.f};
	float _filter_sample_rate{1.f / 800.f};

	perf_counter_t _cycle_perf{perf_alloc(PC_ELAPSED, MODULE_NAME": cycle time")};

	DEFINE_PARAMETERS(
		(ParamBool<px4::params::ATUNE_START>) _param_atune_start,
		(ParamFloat<px4::params::ATUNE_SYSID_AMP>) _param_atune_sysid_amp,

		(ParamFloat<px4::params::IMU_GYRO_CUTOFF>) _param_imu_gyro_cutoff,

		(ParamFloat<px4::params::MC_ROLLRATE_P>) _param_mc_rollrate_p,
		(ParamFloat<px4::params::MC_ROLLRATE_K>) _param_mc_rollrate_k,
		(ParamFloat<px4::params::MC_PITCHRATE_P>) _param_mc_pitchrate_p,
		(ParamFloat<px4::params::MC_PITCHRATE_K>) _param_mc_pitchrate_k
	)
};
