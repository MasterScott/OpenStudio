/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      // These apply to all AirTerminals
      "Zone Air Terminal Sensible Heating Energy",
      "Zone Air Terminal Sensible Heating Rate",
      "Zone Air Terminal Sensible Cooling Energy",
      "Zone Air Terminal Sensible Cooling Rate",

      "Zone Air Terminal VAV Damper Position",
      "Zone Air Terminal Outdoor Air Volume Flow Rate"
    };
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVHeatAndCoolNoReheat","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule);
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::zoneMinimumAirFlowFraction() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule,
                              "AirTerminalSingleDuctVAVHeatAndCoolNoReheat",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction, zoneMinimumAirFlowFraction);
    return result;
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::inletPort() const
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirInlet;
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outletPort() const
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirOutlet;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = inlet;
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              Node inletNode(_model);

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );

              _model.connect( inletNode,
                              inletNode.outletPort(),
                              this->getObject<ModelObject>(),
                              this->inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctVAVHeatAndCoolNoReheat mo = this->getObject<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>();

                thermalZone->addEquipment(mo);
              }

              return true;
            }
          }
        }
      }
    }

    return false;
  }

  std::vector<IdfObject> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);

        break;
      }
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      if( boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject() )
        {
          if( boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort()) )
          {
            _model.connect( source2ModelObject.get(),
                            source2Port.get(),
                            targetModelObject.get(),
                            targetPort.get() );

            inletNode->disconnect();
            inletNode->remove();

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isRemovable() const
  {
    return true;
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosizedMaximumAirFlowRate() const {
    return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosize() {
    autosizeMaximumAirFlowRate();
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedMaximumAirFlowRate();
    if (val) {
      setMaximumAirFlowRate(val.get());
    }

  }

} // detail

AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const Model& model)
  : StraightComponent(AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>());

  autosizeMaximumAirFlowRate();
  setZoneMinimumAirFlowFraction(0.0);
}

IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat);
}

boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->maximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

double AirTerminalSingleDuctVAVHeatAndCoolNoReheat::zoneMinimumAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->zoneMinimumAirFlowFraction();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setZoneMinimumAirFlowFraction(zoneMinimumAirFlowFraction);
}

/// @cond
AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

} // model
} // openstudio
