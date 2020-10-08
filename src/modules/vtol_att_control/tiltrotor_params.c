/****************************************************************************
 *
 *   Copyright (c) 2015 PX4 Development Team. All rights reserved.
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
 * @file tiltrotor_params.c
 * Parameters for vtol attitude controller.
 *
 * @author Roman Bapst <roman@px4.io>
 */

/**
 * Position of tilt servo in mc mode
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 3
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_TILT_MC, 0.0f);

/**
 * Position of tilt servo in transition mode
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 3
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_TILT_TRANS, 0.3f);

/**
 * Position of tilt servo in fw mode
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 3
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_TILT_FW, 1.0f);

/**
 * Tilt actuator control value commanded when disarmed and during the first second after arming.
 *
 * This specific tilt during spin-up is necessary for some systems whose motors otherwise don't
 * spin-up freely.
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 3
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_TILT_SPINUP, 0.0f);

/**
 * Duration of front transition phase 2
 *
 * Time in seconds it should take for the rotors to rotate forward completely from the point
 * when the plane has picked up enough airspeed and is ready to go into fixed wind mode.
 *
 * @unit s
 * @min 0.1
 * @max 5.0
 * @increment 0.01
 * @decimal 3
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_TRANS_P2_DUR, 0.5f);

/**
 * Throttle threshold to switch on alternative motor.
 *
 * The alternative fixed-wing motors are switched on if the throttle setpoint falls below this threshold.
 *
 * @min -1.0
 * @max 1.0
 * @increment 0.01
 * @decimal 2
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_THR_ALTER_ON, -1.0f);

/**
 * Throttle threshold to switch off alternative motor.
 *
 * The alternative fixed-wing motors are switched off if the throttle setpoint raises above this threshold.
 *
 * @min -1.0
 * @max 1.0
 * @increment 0.01
 * @decimal 2
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_THR_ALTER_OFF, -1.0f);

/**
 * Neutral throttle of main motors. Used for throttle scaling in case alternative throttle is used.
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 2
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_THR_N_MAIN, 0.0f);

/**
 * Neutral throttle of alternative motors. Used for throttle scaling in case alternative throttle is used.
 *
 * @min 0.0
 * @max 1.0
 * @increment 0.01
 * @decimal 2
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_THR_N_ALTER, 0.0f);

/**
 * Scle factor for alternative throttle. Used for throttle scaling in case alternative throttle is used.
 *
 * @min 0.0
 * @max 10
 * @increment 0.01
 * @decimal 2
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_THR_ALTER_SC, 1.0f);

/**
 * Pitch actuator compensation when alternative motor is on.
 *
 * Pitch_actuator_offset = VT_ELEV_COMP_OFF + throttle_alt * comp_elev_k / p_dynamic.
 *
 * @min 0.0
 * @max 500.0
 * @increment 0.1
 * @decimal 1
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_ELEV_COMP_K, 0.0f);


/**
 * Static offset added to the elevator control as part of the non-zero thrust line offset compensation for the alternate motor.
 *
 * Pitch_actuator_offset = VT_ELEV_COMP_OFF + throttle_alt * comp_elev_k / p_dynamic.
 *
 * @min -1
 * @max 1
 * @increment 0.1
 * @decimal 1
 * @group VTOL Attitude Control
 */
PARAM_DEFINE_FLOAT(VT_ELEV_COMP_OFF, 0.0f);
