static const u8 sNoneDescription[] = _("Sin habilidad especial.");
static const u8 sStenchDescription[] = _("Ayuda a repeler POKéMON salvajes.");
static const u8 sDrizzleDescription[] = _("Hace que llueva en batalla.");
static const u8 sSpeedBoostDescription[] = _("Sube la VELOCIDAD gradualmente.");
static const u8 sBattleArmorDescription[] = _("Bloquea ataques críticos.");
static const u8 sSturdyDescription[] = _("Anula ataques fulminantes.");
static const u8 sDampDescription[] = _("Evita la autodestrucción.");
static const u8 sLimberDescription[] = _("Evita la parálisis.");
static const u8 sSandVeilDescription[] = _("Sube la evasión en tormentas de arena.");
static const u8 sStaticDescription[] = _("Paraliza al contacto.");
static const u8 sVoltAbsorbDescription[] = _("Transforma electricidad en PS.");
static const u8 sWaterAbsorbDescription[] = _("Convierte agua en PS.");
static const u8 sObliviousDescription[] = _("Evita la atracción.");
static const u8 sCloudNineDescription[] = _("Anula los efectos del clima.");
static const u8 sCompoundEyesDescription[] = _("Aumenta la precisión.");
static const u8 sInsomniaDescription[] = _("Evita el quedarse dormido.");
static const u8 sColorChangeDescription[] = _("Obtiene el tipo del movimiento rival.");
static const u8 sImmunityDescription[] = _("Evita el envenenamiento.");
static const u8 sFlashFireDescription[] = _("Se carga si recibe fuego.");
static const u8 sShieldDustDescription[] = _("Evita efectos secundarios.");
static const u8 sOwnTempoDescription[] = _("Evita la confusión.");
static const u8 sSuctionCupsDescription[] = _("Fija el cuerpo con firmeza.");
static const u8 sIntimidateDescription[] = _("Baja el ATAQUE del rival.");
static const u8 sShadowTagDescription[] = _("Evita el escape del enemigo.");
static const u8 sRoughSkinDescription[] = _("Hiere al tacto.");
static const u8 sWonderGuardDescription[] = _("Solo ataques “muy eficaces” hacen daño.");
static const u8 sLevitateDescription[] = _("No sufre ataques de tipo TIERRA.");
static const u8 sEffectSporeDescription[] = _("Deja esporas al contacto.");
static const u8 sSynchronizeDescription[] = _("Transmite problemas de estado.");
static const u8 sClearBodyDescription[] = _("Evita la baja de estadísticas.");
static const u8 sNaturalCureDescription[] = _("Se cura al salir de la batalla.");
static const u8 sLightningRodDescription[] = _("Frena ataques eléctricos.");
static const u8 sSereneGraceDescription[] = _("Mejora efectos secundarios.");
static const u8 sSwiftSwimDescription[] = _("Sube VELOCIDAD en la lluvia.");
static const u8 sChlorophyllDescription[] = _("Sube VELOCIDAD bajo el sol.");
static const u8 sIlluminateDescription[] = _("Facilita encuentros.");
static const u8 sTraceDescription[] = _("Copia habilidad especial.");
static const u8 sHugePowerDescription[] = _("Aumenta el ATAQUE.");
static const u8 sPoisonPointDescription[] = _("Envenena al enemigo al contacto.");
static const u8 sInnerFocusDescription[] = _("Evita el retroceso.");
static const u8 sMagmaArmorDescription[] = _("Evita el congelamiento.");
static const u8 sWaterVeilDescription[] = _("Evita las quemaduras.");
static const u8 sMagnetPullDescription[] = _("Atrapa POKéMON de tipo ACERO.");
static const u8 sSoundproofDescription[] = _("Evita ataques de sonido.");
static const u8 sRainDishDescription[] = _("Recupera PS en la lluvia.");
static const u8 sSandStreamDescription[] = _("Crea una tormenta de arena.");
static const u8 sPressureDescription[] = _("Aumenta el uso de PP del rival.");
static const u8 sThickFatDescription[] = _("Protege del frío y calor.");
static const u8 sEarlyBirdDescription[] = _("Despierta del sueño rápidamente.");
static const u8 sFlameBodyDescription[] = _("Quema al enemigo al contacto.");
static const u8 sRunAwayDescription[] = _("Facilita el escape.");
static const u8 sKeenEyeDescription[] = _("Evita que la precisión baje.");
static const u8 sHyperCutterDescription[] = _("Evita que baje el ATAQUE.");
static const u8 sPickupDescription[] = _("Puede que tome objetos.");
static const u8 sTruantDescription[] = _("Solo se mueve cada 2 turnos.");
static const u8 sHustleDescription[] = _("Cambia precisión por poder.");
static const u8 sCuteCharmDescription[] = _("Enamora al contacto.");
static const u8 sPlusDescription[] = _("Mejora con la habilidad MENOS.");
static const u8 sMinusDescription[] = _("Mejora con la habilidad MÁS.");
static const u8 sForecastDescription[] = _("Cambia con el clima.");
static const u8 sStickyHoldDescription[] = _("Evita el robo de objetos.");
static const u8 sShedSkinDescription[] = _("Se cura mudando de piel.");
static const u8 sGutsDescription[] = _("Sube el ATAQUE si sufre.");
static const u8 sMarvelScaleDescription[] = _("Sube la DEFENSA si sufre.");
static const u8 sLiquidOozeDescription[] = _("El drenarlo causa daño.");
static const u8 sOvergrowDescription[] = _("Sube ataques de tipo PLANTA en aprietos.");
static const u8 sBlazeDescription[] = _("Sube ataques de tipo FUEGO en aprietos.");
static const u8 sTorrentDescription[] = _("Sube ataques de tipo AGUA en aprietos.");
static const u8 sSwarmDescription[] = _("Sube ataques de tipo INSECTO en aprietos.");
static const u8 sRockHeadDescription[] = _("Evita el daño por retroceso.");
static const u8 sDroughtDescription[] = _("Trae luz solar a la batalla.");
static const u8 sArenaTrapDescription[] = _("Evita el escape.");
static const u8 sVitalSpiritDescription[] = _("Evita quedarse dormido.");
static const u8 sWhiteSmokeDescription[] = _("Evita la baja de estadísticas.");
static const u8 sPurePowerDescription[] = _("Aumenta el ATAQUE.");
static const u8 sShellArmorDescription[] = _("Bloquea ataques críticos.");
static const u8 sCacophonyDescription[] = _("Evita ataques de sonido.");
static const u8 sAirLockDescription[] = _("Anula los efectos del clima.");

const u8 *const gAbilityDescriptionPointers[ABILITIES_COUNT] =
{
    [ABILITY_NONE] = sNoneDescription,
    [ABILITY_STENCH] = sStenchDescription,
    [ABILITY_DRIZZLE] = sDrizzleDescription,
    [ABILITY_SPEED_BOOST] = sSpeedBoostDescription,
    [ABILITY_BATTLE_ARMOR] = sBattleArmorDescription,
    [ABILITY_STURDY] = sSturdyDescription,
    [ABILITY_DAMP] = sDampDescription,
    [ABILITY_LIMBER] = sLimberDescription,
    [ABILITY_SAND_VEIL] = sSandVeilDescription,
    [ABILITY_STATIC] = sStaticDescription,
    [ABILITY_VOLT_ABSORB] = sVoltAbsorbDescription,
    [ABILITY_WATER_ABSORB] = sWaterAbsorbDescription,
    [ABILITY_OBLIVIOUS] = sObliviousDescription,
    [ABILITY_CLOUD_NINE] = sCloudNineDescription,
    [ABILITY_COMPOUND_EYES] = sCompoundEyesDescription,
    [ABILITY_INSOMNIA] = sInsomniaDescription,
    [ABILITY_COLOR_CHANGE] = sColorChangeDescription,
    [ABILITY_IMMUNITY] = sImmunityDescription,
    [ABILITY_FLASH_FIRE] = sFlashFireDescription,
    [ABILITY_SHIELD_DUST] = sShieldDustDescription,
    [ABILITY_OWN_TEMPO] = sOwnTempoDescription,
    [ABILITY_SUCTION_CUPS] = sSuctionCupsDescription,
    [ABILITY_INTIMIDATE] = sIntimidateDescription,
    [ABILITY_SHADOW_TAG] = sShadowTagDescription,
    [ABILITY_ROUGH_SKIN] = sRoughSkinDescription,
    [ABILITY_WONDER_GUARD] = sWonderGuardDescription,
    [ABILITY_LEVITATE] = sLevitateDescription,
    [ABILITY_EFFECT_SPORE] = sEffectSporeDescription,
    [ABILITY_SYNCHRONIZE] = sSynchronizeDescription,
    [ABILITY_CLEAR_BODY] = sClearBodyDescription,
    [ABILITY_NATURAL_CURE] = sNaturalCureDescription,
    [ABILITY_LIGHTNING_ROD] = sLightningRodDescription,
    [ABILITY_SERENE_GRACE] = sSereneGraceDescription,
    [ABILITY_SWIFT_SWIM] = sSwiftSwimDescription,
    [ABILITY_CHLOROPHYLL] = sChlorophyllDescription,
    [ABILITY_ILLUMINATE] = sIlluminateDescription,
    [ABILITY_TRACE] = sTraceDescription,
    [ABILITY_HUGE_POWER] = sHugePowerDescription,
    [ABILITY_POISON_POINT] = sPoisonPointDescription,
    [ABILITY_INNER_FOCUS] = sInnerFocusDescription,
    [ABILITY_MAGMA_ARMOR] = sMagmaArmorDescription,
    [ABILITY_WATER_VEIL] = sWaterVeilDescription,
    [ABILITY_MAGNET_PULL] = sMagnetPullDescription,
    [ABILITY_SOUNDPROOF] = sSoundproofDescription,
    [ABILITY_RAIN_DISH] = sRainDishDescription,
    [ABILITY_SAND_STREAM] = sSandStreamDescription,
    [ABILITY_PRESSURE] = sPressureDescription,
    [ABILITY_THICK_FAT] = sThickFatDescription,
    [ABILITY_EARLY_BIRD] = sEarlyBirdDescription,
    [ABILITY_FLAME_BODY] = sFlameBodyDescription,
    [ABILITY_RUN_AWAY] = sRunAwayDescription,
    [ABILITY_KEEN_EYE] = sKeenEyeDescription,
    [ABILITY_HYPER_CUTTER] = sHyperCutterDescription,
    [ABILITY_PICKUP] = sPickupDescription,
    [ABILITY_TRUANT] = sTruantDescription,
    [ABILITY_HUSTLE] = sHustleDescription,
    [ABILITY_CUTE_CHARM] = sCuteCharmDescription,
    [ABILITY_PLUS] = sPlusDescription,
    [ABILITY_MINUS] = sMinusDescription,
    [ABILITY_FORECAST] = sForecastDescription,
    [ABILITY_STICKY_HOLD] = sStickyHoldDescription,
    [ABILITY_SHED_SKIN] = sShedSkinDescription,
    [ABILITY_GUTS] = sGutsDescription,
    [ABILITY_MARVEL_SCALE] = sMarvelScaleDescription,
    [ABILITY_LIQUID_OOZE] = sLiquidOozeDescription,
    [ABILITY_OVERGROW] = sOvergrowDescription,
    [ABILITY_BLAZE] = sBlazeDescription,
    [ABILITY_TORRENT] = sTorrentDescription,
    [ABILITY_SWARM] = sSwarmDescription,
    [ABILITY_ROCK_HEAD] = sRockHeadDescription,
    [ABILITY_DROUGHT] = sDroughtDescription,
    [ABILITY_ARENA_TRAP] = sArenaTrapDescription,
    [ABILITY_VITAL_SPIRIT] = sVitalSpiritDescription,
    [ABILITY_WHITE_SMOKE] = sWhiteSmokeDescription,
    [ABILITY_PURE_POWER] = sPurePowerDescription,
    [ABILITY_SHELL_ARMOR] = sShellArmorDescription,
    [ABILITY_CACOPHONY] = sCacophonyDescription,
    [ABILITY_AIR_LOCK] = sAirLockDescription,
};

const u8 gAbilityNames[ABILITIES_COUNT][ABILITY_NAME_LENGTH + 1] =
{
    [ABILITY_NONE] = _("-------"),
    [ABILITY_STENCH] = _("HEDOR"),
    [ABILITY_DRIZZLE] = _("LLOVIZNA"),
    [ABILITY_SPEED_BOOST] = _("IMPULSO"),
    [ABILITY_BATTLE_ARMOR] = _("ARMAD. BAT."),
    [ABILITY_STURDY] = _("ROBUSTEZ"),
    [ABILITY_DAMP] = _("HUMEDAD"),
    [ABILITY_LIMBER] = _("FLEXIBILIDAD"),
    [ABILITY_SAND_VEIL] = _("VELO ARENA"),
    [ABILITY_STATIC] = _("ELEC. ESTÁT."),
    [ABILITY_VOLT_ABSORB] = _("ABSOR. ELEC."),
    [ABILITY_WATER_ABSORB] = _("ABSOR. AGUA"),
    [ABILITY_OBLIVIOUS] = _("DESPISTADO"),
    [ABILITY_CLOUD_NINE] = _("CLIMA IDEAL"),
    [ABILITY_COMPOUND_EYES] = _("OJOCOMPUESTO"),
    [ABILITY_INSOMNIA] = _("INSOMNIO"),
    [ABILITY_COLOR_CHANGE] = _("CAMBIO COLOR"),
    [ABILITY_IMMUNITY] = _("INMUNIDAD"),
    [ABILITY_FLASH_FIRE] = _("ABSOR. FUEGO"),
    [ABILITY_SHIELD_DUST] = _("POLVO ESCUDO"),
    [ABILITY_OWN_TEMPO] = _("RITMO PROPIO"),
    [ABILITY_SUCTION_CUPS] = _("VENTOSAS"),
    [ABILITY_INTIMIDATE] = _("INTIMIDACIÓN"),
    [ABILITY_SHADOW_TAG] = _("TRAMPASOMBRA"),
    [ABILITY_ROUGH_SKIN] = _("PIEL DURA"),
    [ABILITY_WONDER_GUARD] = _("SUPERDEFENSA"),
    [ABILITY_LEVITATE] = _("LEVITACIÓN"),
    [ABILITY_EFFECT_SPORE] = _("ESPORAS"),
    [ABILITY_SYNCHRONIZE] = _("SINCRONÍA"),
    [ABILITY_CLEAR_BODY] = _("CUERPO PURO"),
    [ABILITY_NATURAL_CURE] = _("CURA NATURAL"),
    [ABILITY_LIGHTNING_ROD] = _("PARARRAYOS"),
    [ABILITY_SERENE_GRACE] = _("DICHA"),
    [ABILITY_SWIFT_SWIM] = _("NADO RÁPIDO"),
    [ABILITY_CHLOROPHYLL] = _("CLOROFILA"),
    [ABILITY_ILLUMINATE] = _("ILUMINACIÓN"),
    [ABILITY_TRACE] = _("CALCO"),
    [ABILITY_HUGE_POWER] = _("GRAN PODER"),
    [ABILITY_POISON_POINT] = _("PUNTO TÓXICO"),
    [ABILITY_INNER_FOCUS] = _("VOLUNTAD"),
    [ABILITY_MAGMA_ARMOR] = _("ESCUDO MAGMA"),
    [ABILITY_WATER_VEIL] = _("VELO AGUA"),
    [ABILITY_MAGNET_PULL] = _("MAGNETISMO"),
    [ABILITY_SOUNDPROOF] = _("INSONORO"),
    [ABILITY_RAIN_DISH] = _("CURA LLUVIA"),
    [ABILITY_SAND_STREAM] = _("CHORRO ARENA"),
    [ABILITY_PRESSURE] = _("PRESIÓN"),
    [ABILITY_THICK_FAT] = _("SEBO"),
    [ABILITY_EARLY_BIRD] = _("MADRUGAR"),
    [ABILITY_FLAME_BODY] = _("CUERPO LLAMA"),
    [ABILITY_RUN_AWAY] = _("FUGA"),
    [ABILITY_KEEN_EYE] = _("PERSPICACIA"),
    [ABILITY_HYPER_CUTTER] = _("HIPERCORTE"),
    [ABILITY_PICKUP] = _("RECOGIDA"),
    [ABILITY_TRUANT] = _("AUSENTE"),
    [ABILITY_HUSTLE] = _("ENTUSIASMO"),
    [ABILITY_CUTE_CHARM] = _("GRAN ENCANTO"),
    [ABILITY_PLUS] = _("MÁS"),
    [ABILITY_MINUS] = _("MENOS"),
    [ABILITY_FORECAST] = _("PRONÓSTICO"),
    [ABILITY_STICKY_HOLD] = _("VISCOSIDAD"),
    [ABILITY_SHED_SKIN] = _("MUDA DE PIEL"),
    [ABILITY_GUTS] = _("AGALLAS"),
    [ABILITY_MARVEL_SCALE] = _("ESCAMA ESP."),
    [ABILITY_LIQUID_OOZE] = _("LODO LÍQUIDO"),
    [ABILITY_OVERGROW] = _("ESPESURA"),
    [ABILITY_BLAZE] = _("LLAMAS FURIA"),
    [ABILITY_TORRENT] = _("TORRENTE"),
    [ABILITY_SWARM] = _("ENJAMBRE"),
    [ABILITY_ROCK_HEAD] = _("CABEZA ROCA"),
    [ABILITY_DROUGHT] = _("SEQUÍA"),
    [ABILITY_ARENA_TRAP] = _("TRAMPA ARENA"),
    [ABILITY_VITAL_SPIRIT] = _("ESPÍR. VITAL"),
    [ABILITY_WHITE_SMOKE] = _("HUMO BLANCO"),
    [ABILITY_PURE_POWER] = _("PODER PURO"),
    [ABILITY_SHELL_ARMOR] = _("CAPARAZÓN"),
    [ABILITY_CACOPHONY] = _("CACOFONÍA"),
    [ABILITY_AIR_LOCK] = _("BLOQUEO AIRE"),
};
