const u8 gMoveNames[MOVES_COUNT][MOVE_NAME_LENGTH + 1] = {
    [MOVE_NONE]          = _("-$$$$$$"),
    [MOVE_POUND]         = _("GOLPE"),
    [MOVE_KARATE_CHOP]   = _("GOLPE KÁRATE"),
    [MOVE_DOUBLE_SLAP]   = _("BOFETADAS"),
    [MOVE_COMET_PUNCH]   = _("PUÑO COMETA"),
    [MOVE_MEGA_PUNCH]    = _("MEGA PUÑO"),
    [MOVE_PAY_DAY]       = _("DÍA DE PAGO"),
    [MOVE_FIRE_PUNCH]    = _("PUÑO FUEGO"),
    [MOVE_ICE_PUNCH]     = _("PUÑO HIELO"),
    [MOVE_THUNDER_PUNCH] = _("PUÑO TRUENO"),
    [MOVE_SCRATCH]       = _("ARAÑAZO"),
    [MOVE_VICE_GRIP]     = _("FUERZA GARRA"),
    [MOVE_GUILLOTINE]    = _("GUILLOTINA"),
    [MOVE_RAZOR_WIND]    = _("V. CORTANTE"),
    [MOVE_SWORDS_DANCE]  = _("DANZA ESPADA"),
    [MOVE_CUT]           = _("CORTE"),
    [MOVE_GUST]          = _("RÁFAGA"),
    [MOVE_WING_ATTACK]   = _("ATAQUE ALA"),
    [MOVE_WHIRLWIND]     = _("TORBELLINO"),
    [MOVE_FLY]           = _("VUELO"),
    [MOVE_BIND]          = _("ATADURA"),
    [MOVE_SLAM]          = _("GOLPAZO"),
    [MOVE_VINE_WHIP]     = _("LÁTIGO CEPA"),
    [MOVE_STOMP]         = _("PISOTÓN"),
    [MOVE_DOUBLE_KICK]   = _("PATADA DOBLE"),
    [MOVE_MEGA_KICK]     = _("MEGA PATADA"),
    [MOVE_JUMP_KICK]     = _("PATADA SALTO"),
    [MOVE_ROLLING_KICK]  = _("PATADA GIRO"),
    [MOVE_SAND_ATTACK]   = _("ATAQUE ARENA"),
    [MOVE_HEADBUTT]      = _("GOLPE CABEZA"),
    [MOVE_HORN_ATTACK]   = _("CORNADA"),
    [MOVE_FURY_ATTACK]   = _("ATAQUE FURIA"),
    [MOVE_HORN_DRILL]    = _("PERFORADOR"),
    [MOVE_TACKLE]        = _("TACLEADA"),
    [MOVE_BODY_SLAM]     = _("GOLPE CUERPO"),
    [MOVE_WRAP]          = _("ENVOLTURA"),
    [MOVE_TAKE_DOWN]     = _("DERRIBO"),
    [MOVE_THRASH]        = _("PALIZA"),
    [MOVE_DOUBLE_EDGE]   = _("DOBLE FILO"),
    [MOVE_TAIL_WHIP]     = _("MENEO COLA"),
    [MOVE_POISON_STING]  = _("PIQUETE VEN."),
    [MOVE_TWINEEDLE]     = _("PIQUETEDOBLE"),
    [MOVE_PIN_MISSILE]   = _("MISIL. AGUJA"),
    [MOVE_LEER]          = _("MIRADA"),
    [MOVE_BITE]          = _("MORDIDA"),
    [MOVE_GROWL]         = _("GRUÑIDO"),
    [MOVE_ROAR]          = _("RUGIDO"),
    [MOVE_SING]          = _("CANTO"),
    [MOVE_SUPERSONIC]    = _("SUPERSÓNICO"),
    [MOVE_SONIC_BOOM]    = _("EXPLOSIÓN S."),
    [MOVE_DISABLE]       = _("ANULACIÓN"),
    [MOVE_ACID]          = _("ÁCIDO"),
    [MOVE_EMBER]         = _("BRASAS"),
    [MOVE_FLAMETHROWER]  = _("LANZALLAMAS"),
    [MOVE_MIST]          = _("NIEBLA"),
    [MOVE_WATER_GUN]     = _("CHORRO AGUA"),
    [MOVE_HYDRO_PUMP]    = _("HIDROBOMBA"),
    [MOVE_SURF]          = _("SURF"),
    [MOVE_ICE_BEAM]      = _("RAYO HIELO"),
    [MOVE_BLIZZARD]      = _("VENTISCA"),
    [MOVE_PSYBEAM]       = _("PSICORRAYO"),
    [MOVE_BUBBLE_BEAM]   = _("RAYO BURBUJA"),
    [MOVE_AURORA_BEAM]   = _("RAYO AURORA"),
    [MOVE_HYPER_BEAM]    = _("HIPERRAYO"),
    [MOVE_PECK]          = _("PICOTAZO"),
    [MOVE_DRILL_PECK]    = _("PICO TALADRO"),
    [MOVE_SUBMISSION]    = _("SUMISIÓN"),
    [MOVE_LOW_KICK]      = _("PATADA BAJA"),
    [MOVE_COUNTER]       = _("CONTRAATAQUE"),
    [MOVE_SEISMIC_TOSS]  = _("TIRO SÍSMICO"),
    [MOVE_STRENGTH]      = _("FUERZA"),
    [MOVE_ABSORB]        = _("ABSORCIÓN"),
    [MOVE_MEGA_DRAIN]    = _("MEGADRENADO"),
    [MOVE_LEECH_SEED]    = _("DRENADORAS"),
    [MOVE_GROWTH]        = _("CRECIMIENTO"),
    [MOVE_RAZOR_LEAF]    = _("HOJA AFILADA"),
    [MOVE_SOLAR_BEAM]    = _("RAYO SOLAR"),
    [MOVE_POISON_POWDER] = _("POLVO VENENO"),
    [MOVE_STUN_SPORE]    = _("POLVO PARÁL."),
    [MOVE_SLEEP_POWDER]  = _("POLVO SUEÑO"),
    [MOVE_PETAL_DANCE]   = _("DANZA PÉTALO"),
    [MOVE_STRING_SHOT]   = _("DISPARO SEDA"),
    [MOVE_DRAGON_RAGE]   = _("FURIA DRAGÓN"),
    [MOVE_FIRE_SPIN]     = _("GIRO FUEGO"),
    [MOVE_THUNDER_SHOCK] = _("IMPACTRUENO"),
    [MOVE_THUNDERBOLT]   = _("ATACTRUENO"),
    [MOVE_THUNDER_WAVE]  = _("ONDA TRUENO"),
    [MOVE_THUNDER]       = _("TRUENO"),
    [MOVE_ROCK_THROW]    = _("LANZARROCAS"),
    [MOVE_EARTHQUAKE]    = _("TERREMOTO"),
    [MOVE_FISSURE]       = _("FISURA"),
    [MOVE_DIG]           = _("CAVAR"),
    [MOVE_TOXIC]         = _("TÓXICO"),
    [MOVE_CONFUSION]     = _("CONFUSIÓN"),
    [MOVE_PSYCHIC]       = _("PSÍQUICO"),
    [MOVE_HYPNOSIS]      = _("HIPNOSIS"),
    [MOVE_MEDITATE]      = _("MEDITAR"),
    [MOVE_AGILITY]       = _("AGILIDAD"),
    [MOVE_QUICK_ATTACK]  = _("AT. RÁPIDO"),
    [MOVE_RAGE]          = _("FURIA"),
    [MOVE_TELEPORT]      = _("TELETRANSP."),
    [MOVE_NIGHT_SHADE]   = _("TINIEBLAS"),
    [MOVE_MIMIC]         = _("MÍMICA"),
    [MOVE_SCREECH]       = _("ALARIDO"),
    [MOVE_DOUBLE_TEAM]   = _("DOBLE EQUIPO"),
    [MOVE_RECOVER]       = _("RECUPERACIÓN"),
    [MOVE_HARDEN]        = _("ENDURECER"),
    [MOVE_MINIMIZE]      = _("MINIMIZAR"),
    [MOVE_SMOKESCREEN]   = _("PANTALLAHUMO"),
    [MOVE_CONFUSE_RAY]   = _("RAYO CONFUSO"),
    [MOVE_WITHDRAW]      = _("REFUGIO"),
    [MOVE_DEFENSE_CURL]  = _("RIZO DEFENSA"),
    [MOVE_BARRIER]       = _("BARRERA"),
    [MOVE_LIGHT_SCREEN]  = _("PANTALLA LUZ"),
    [MOVE_HAZE]          = _("NEBLINA"),
    [MOVE_REFLECT]       = _("REFLEJO"),
    [MOVE_FOCUS_ENERGY]  = _("FOCO ENERGÍA"),
    [MOVE_BIDE]          = _("PERSEVERAR"),
    [MOVE_METRONOME]     = _("METRÓNOMO"),
    [MOVE_MIRROR_MOVE]   = _("MOV. ESPEJO"),
    [MOVE_SELF_DESTRUCT] = _("AUTODESTRUC."),
    [MOVE_EGG_BOMB]      = _("BOMBA HUEVO"),
    [MOVE_LICK]          = _("LENGÜETAZO"),
    [MOVE_SMOG]          = _("HUMO"),
    [MOVE_SLUDGE]        = _("DESECHOS"),
    [MOVE_BONE_CLUB]     = _("GOLPE HUESO"),
    [MOVE_FIRE_BLAST]    = _("LLAMARADA"),
    [MOVE_WATERFALL]     = _("CASCADA"),
    [MOVE_CLAMP]         = _("SUJETAR"),
    [MOVE_SWIFT]         = _("VELOCIDAD"),
    [MOVE_SKULL_BASH]    = _("CABEZAZO"),
    [MOVE_SPIKE_CANNON]  = _("CAÑON PICOS"),
    [MOVE_CONSTRICT]     = _("APRETÓN"),
    [MOVE_AMNESIA]       = _("AMNESIA"),
    [MOVE_KINESIS]       = _("KINÉTICO"),
    [MOVE_SOFT_BOILED]   = _("HUEVO COCIDO"),
    [MOVE_HI_JUMP_KICK]  = _("PAT. S. ALTO"),
    [MOVE_GLARE]         = _("MIRADAREPTIL"),
    [MOVE_DREAM_EATER]   = _("COME SUEÑOS"),
    [MOVE_POISON_GAS]    = _("GAS VENENOSO"),
    [MOVE_BARRAGE]       = _("BOMBARDEO"),
    [MOVE_LEECH_LIFE]    = _("CHUPA VIDA"),
    [MOVE_LOVELY_KISS]   = _("BESO AMOROSO"),
    [MOVE_SKY_ATTACK]    = _("ATAQUE AÉREO"),
    [MOVE_TRANSFORM]     = _("TRANSFORM."),
    [MOVE_BUBBLE]        = _("BURBUJA"),
    [MOVE_DIZZY_PUNCH]   = _("PUÑO MAREO"),
    [MOVE_SPORE]         = _("ESPORAS"),
    [MOVE_FLASH]         = _("DESTELLO"),
    [MOVE_PSYWAVE]       = _("PSICOONDA"),
    [MOVE_SPLASH]        = _("SALTO"),
    [MOVE_ACID_ARMOR]    = _("ARMAD. ÁCIDA"),
    [MOVE_CRABHAMMER]    = _("MARTILLAZO"),
    [MOVE_EXPLOSION]     = _("EXPLOSIÓN"),
    [MOVE_FURY_SWIPES]   = _("GARRAS FURIA"),
    [MOVE_BONEMERANG]    = _("HUESORÁN"),
    [MOVE_REST]          = _("DESCANSO"),
    [MOVE_ROCK_SLIDE]    = _("AVALANCHA"),
    [MOVE_HYPER_FANG]    = _("HIP.COLMILLO"),
    [MOVE_SHARPEN]       = _("AFILAR"),
    [MOVE_CONVERSION]    = _("CONVERSIÓN"),
    [MOVE_TRI_ATTACK]    = _("TRIATAQUE"),
    [MOVE_SUPER_FANG]    = _("SUP.COLMILLO"),
    [MOVE_SLASH]         = _("CUCHILLADA"),
    [MOVE_SUBSTITUTE]    = _("SUSTITUTO"),
    [MOVE_STRUGGLE]      = _("FORCEJEO"),
    [MOVE_SKETCH]        = _("BOCETO"),
    [MOVE_TRIPLE_KICK]   = _("PATADATRIPLE"),
    [MOVE_THIEF]         = _("LADRÓN"),
    [MOVE_SPIDER_WEB]    = _("TELARAÑA"),
    [MOVE_MIND_READER]   = _("TELÉPATA"),
    [MOVE_NIGHTMARE]     = _("PESADILLA"),
    [MOVE_FLAME_WHEEL]   = _("RUEDA FUEGO"),
    [MOVE_SNORE]         = _("RONQUIDO"),
    [MOVE_CURSE]         = _("MALDICIÓN"),
    [MOVE_FLAIL]         = _("AZOTAR"),
    [MOVE_CONVERSION_2]  = _("CONVERSIÓN 2"),
    [MOVE_AEROBLAST]     = _("AEROCHORRO"),
    [MOVE_COTTON_SPORE]  = _("ESPORAGODÓN"),
    [MOVE_REVERSAL]      = _("INVERSIÓN"),
    [MOVE_SPITE]         = _("RENCOR"),
    [MOVE_POWDER_SNOW]   = _("POLVO NIEVE"),
    [MOVE_PROTECT]       = _("PROTECCIÓN"),
    [MOVE_MACH_PUNCH]    = _("PUÑO VELOZ"),
    [MOVE_SCARY_FACE]    = _("CARA MIEDO"),
    [MOVE_FAINT_ATTACK]  = _("ATAQUE FINTA"),
    [MOVE_SWEET_KISS]    = _("BESO DULCE"),
    [MOVE_BELLY_DRUM]    = _("TAMBOR"),
    [MOVE_SLUDGE_BOMB]   = _("BOMBA LODO"),
    [MOVE_MUD_SLAP]      = _("GOLPE LODO"),
    [MOVE_OCTAZOOKA]     = _("OCTAZOOKA"),
    [MOVE_SPIKES]        = _("PÚAS"),
    [MOVE_ZAP_CANNON]    = _("ELECTROCAÑON"),
    [MOVE_FORESIGHT]     = _("VISIÓN"),
    [MOVE_DESTINY_BOND]  = _("LAZO DESTINO"),
    [MOVE_PERISH_SONG]   = _("CANTO MORTAL"),
    [MOVE_ICY_WIND]      = _("VIENTO HIELO"),
    [MOVE_DETECT]        = _("DETECCIÓN"),
    [MOVE_BONE_RUSH]     = _("HUESO VELOZ"),
    [MOVE_LOCK_ON]       = _("FIJAR BLANCO"),
    [MOVE_OUTRAGE]       = _("ENFADO"),
    [MOVE_SANDSTORM]     = _("TORM. ARENA"),
    [MOVE_GIGA_DRAIN]    = _("GIGADRENADO"),
    [MOVE_ENDURE]        = _("RESISTIR"),
    [MOVE_CHARM]         = _("ENCANTO"),
    [MOVE_ROLLOUT]       = _("RODADA"),
    [MOVE_FALSE_SWIPE]   = _("CORTE FALSO"),
    [MOVE_SWAGGER]       = _("DESCONTROL"),
    [MOVE_MILK_DRINK]    = _("BEBE LECHE"),
    [MOVE_SPARK]         = _("CHISPA"),
    [MOVE_FURY_CUTTER]   = _("CORTE FURIA"),
    [MOVE_STEEL_WING]    = _("ALA DE ACERO"),
    [MOVE_MEAN_LOOK]     = _("MIRADA FIJA"),
    [MOVE_ATTRACT]       = _("ATRACCIÓN"),
    [MOVE_SLEEP_TALK]    = _("SONÁMBULO"),
    [MOVE_HEAL_BELL]     = _("CAMPANA CURA"),
    [MOVE_RETURN]        = _("RETRIBUCIÓN"),
    [MOVE_PRESENT]       = _("OBSEQUIO"),
    [MOVE_FRUSTRATION]   = _("FRUSTRACIÓN"),
    [MOVE_SAFEGUARD]     = _("SALVAGUARDA"),
    [MOVE_PAIN_SPLIT]    = _("DIVIDE DOLOR"),
    [MOVE_SACRED_FIRE]   = _("FUEGOSAGRADO"),
    [MOVE_MAGNITUDE]     = _("MAGNITUD"),
    [MOVE_DYNAMIC_PUNCH] = _("PUÑODINÁMICO"),
    [MOVE_MEGAHORN]      = _("MEGA CUERNO"),
    [MOVE_DRAGON_BREATH] = _("ALIENTO DRA."),
    [MOVE_BATON_PASS]    = _("RELEVO"),
    [MOVE_ENCORE]        = _("OTRA VEZ"),
    [MOVE_PURSUIT]       = _("PERSECUCIÓN"),
    [MOVE_RAPID_SPIN]    = _("GIRO RÁPIDO"),
    [MOVE_SWEET_SCENT]   = _("DULCE AROMA"),
    [MOVE_IRON_TAIL]     = _("COLA HIERRO"),
    [MOVE_METAL_CLAW]    = _("GARRA METAL"),
    [MOVE_VITAL_THROW]   = _("TIRO VITAL"),
    [MOVE_MORNING_SUN]   = _("SOL MATUTINO"),
    [MOVE_SYNTHESIS]     = _("SÍNTESIS"),
    [MOVE_MOONLIGHT]     = _("LUZ LUNAR"),
    [MOVE_HIDDEN_POWER]  = _("PODER OCULTO"),
    [MOVE_CROSS_CHOP]    = _("TAJO CRUZADO"),
    [MOVE_TWISTER]       = _("CICLÓN"),
    [MOVE_RAIN_DANCE]    = _("DANZA LLUVIA"),
    [MOVE_SUNNY_DAY]     = _("DÍA SOLEADO"),
    [MOVE_CRUNCH]        = _("TRITURAR"),
    [MOVE_MIRROR_COAT]   = _("MANTO ESPEJO"),
    [MOVE_PSYCH_UP]      = _("MÁS PSIQUE"),
    [MOVE_EXTREME_SPEED] = _("VEL. EXTREMA"),
    [MOVE_ANCIENT_POWER] = _("PODER PASADO"),
    [MOVE_SHADOW_BALL]   = _("BOLA SOMBRA"),
    [MOVE_FUTURE_SIGHT]  = _("VISTA FUTURA"),
    [MOVE_ROCK_SMASH]    = _("ROMPE ROCAS"),
    [MOVE_WHIRLPOOL]     = _("REMOLINO"),
    [MOVE_BEAT_UP]       = _("AT. GRUPAL"),
    [MOVE_FAKE_OUT]      = _("SORPRESA"),
    [MOVE_UPROAR]        = _("ALBOROTO"),
    [MOVE_STOCKPILE]     = _("RESERVAS"),
    [MOVE_SPIT_UP]       = _("ESCUPIDA"),
    [MOVE_SWALLOW]       = _("TRAGAR"),
    [MOVE_HEAT_WAVE]     = _("ONDA CALOR"),
    [MOVE_HAIL]          = _("GRANIZO"),
    [MOVE_TORMENT]       = _("TORMENTO"),
    [MOVE_FLATTER]       = _("HALAGO"),
    [MOVE_WILL_O_WISP]   = _("FUEGO FAUTO"),
    [MOVE_MEMENTO]       = _("LEGADO"),
    [MOVE_FACADE]        = _("FACHADA"),
    [MOVE_FOCUS_PUNCH]   = _("PUÑO CERTERO"),
    [MOVE_SMELLING_SALT] = _("ESTÍMULO"),
    [MOVE_FOLLOW_ME]     = _("SEÑUELO"),
    [MOVE_NATURE_POWER]  = _("ADAPTACIÓN"),
    [MOVE_CHARGE]        = _("CARGA"),
    [MOVE_TAUNT]         = _("MOFA"),
    [MOVE_HELPING_HAND]  = _("REFUERZO"),
    [MOVE_TRICK]         = _("TRUCO"),
    [MOVE_ROLE_PLAY]     = _("IMITACIÓN"),
    [MOVE_WISH]          = _("DESEO"),
    [MOVE_ASSIST]        = _("AYUDA"),
    [MOVE_INGRAIN]       = _("ARRAIGO"),
    [MOVE_SUPERPOWER]    = _("SUPERPODER"),
    [MOVE_MAGIC_COAT]    = _("CAPA MÁGICA"),
    [MOVE_RECYCLE]       = _("RECICLAJE"),
    [MOVE_REVENGE]       = _("VENGANZA"),
    [MOVE_BRICK_BREAK]   = _("KARATAZO"),
    [MOVE_YAWN]          = _("BOSTEZO"),
    [MOVE_KNOCK_OFF]     = _("DESARME"),
    [MOVE_ENDEAVOR]      = _("ESFUERZO"),
    [MOVE_ERUPTION]      = _("ERUPCIÓN"),
    [MOVE_SKILL_SWAP]    = _("INTERCAMBIO"),
    [MOVE_IMPRISON]      = _("SELLAR"),
    [MOVE_REFRESH]       = _("RENOVAR"),
    [MOVE_GRUDGE]        = _("RABIA"),
    [MOVE_SNATCH]        = _("ROBO"),
    [MOVE_SECRET_POWER]  = _("PODER OCULTO"),
    [MOVE_DIVE]          = _("BUCEO"),
    [MOVE_ARM_THRUST]    = _("BRAZADA"),
    [MOVE_CAMOUFLAGE]    = _("CAMUFLAJE"),
    [MOVE_TAIL_GLOW]     = _("COLA DE LUZ"),
    [MOVE_LUSTER_PURGE]  = _("RESPLANDOR"),
    [MOVE_MIST_BALL]     = _("BOLA NIEBLA"),
    [MOVE_FEATHER_DANCE] = _("DANZA PLUMAS"),
    [MOVE_TEETER_DANCE]  = _("DANZA CAOS"),
    [MOVE_BLAZE_KICK]    = _("PATADA FUEGO"),
    [MOVE_MUD_SPORT]     = _("CHAPOTEOLODO"),
    [MOVE_ICE_BALL]      = _("BOLA HIELO"),
    [MOVE_NEEDLE_ARM]    = _("BRAZO AGUJA"),
    [MOVE_SLACK_OFF]     = _("RELAJAR"),
    [MOVE_HYPER_VOICE]   = _("HIPER VOZ"),
    [MOVE_POISON_FANG]   = _("COLMILLO VEN"),
    [MOVE_CRUSH_CLAW]    = _("GARRA BRUTAL"),
    [MOVE_BLAST_BURN]    = _("ANILLO FUEGO"),
    [MOVE_HYDRO_CANNON]  = _("HIDROCAÑON"),
    [MOVE_METEOR_MASH]   = _("PUÑO METEORO"),
    [MOVE_ASTONISH]      = _("IMPRESIÓN"),
    [MOVE_WEATHER_BALL]  = _("METEOROBOLA"),
    [MOVE_AROMATHERAPY]  = _("AROMATERAPIA"),
    [MOVE_FAKE_TEARS]    = _("LLANTO FALSO"),
    [MOVE_AIR_CUTTER]    = _("AIRE AFILADO"),
    [MOVE_OVERHEAT]      = _("SOFOCO"),
    [MOVE_ODOR_SLEUTH]   = _("RASTREO"),
    [MOVE_ROCK_TOMB]     = _("TUMBA ROCAS"),
    [MOVE_SILVER_WIND]   = _("VIENTO PLATA"),
    [MOVE_METAL_SOUND]   = _("ECO METÁLICO"),
    [MOVE_GRASS_WHISTLE] = _("SONIDO HOJA"),
    [MOVE_TICKLE]        = _("COSQUILLAS"),
    [MOVE_COSMIC_POWER]  = _("MASA CÓSMICA"),
    [MOVE_WATER_SPOUT]   = _("SALPICAR"),
    [MOVE_SIGNAL_BEAM]   = _("RAYO SEÑAL"),
    [MOVE_SHADOW_PUNCH]  = _("PUÑO SOMBRA"),
    [MOVE_EXTRASENSORY]  = _("PARANORMAL"),
    [MOVE_SKY_UPPERCUT]  = _("GANCHO ALTO"),
    [MOVE_SAND_TOMB]     = _("TUMBA ARENA"),
    [MOVE_SHEER_COLD]    = _("FRÍO EXTREMO"),
    [MOVE_MUDDY_WATER]   = _("AGUA LODOSA"),
    [MOVE_BULLET_SEED]   = _("BALASSEMILLA"),
    [MOVE_AERIAL_ACE]    = _("AS AÉREO"),
    [MOVE_ICICLE_SPEAR]  = _("CARÁMBANOS"),
    [MOVE_IRON_DEFENSE]  = _("DEF. FÉRREA"),
    [MOVE_BLOCK]         = _("BLOQUEO"),
    [MOVE_HOWL]          = _("AULLIDO"),
    [MOVE_DRAGON_CLAW]   = _("GARRA DRAGÓN"),
    [MOVE_FRENZY_PLANT]  = _("PLANTA FEROZ"),
    [MOVE_BULK_UP]       = _("CORPULENCIA"),
    [MOVE_BOUNCE]        = _("REBOTE"),
    [MOVE_MUD_SHOT]      = _("DISPARO LODO"),
    [MOVE_POISON_TAIL]   = _("COLA VENENO"),
    [MOVE_COVET]         = _("ANTOJO"),
    [MOVE_VOLT_TACKLE]   = _("TAC. VOLTIOS"),
    [MOVE_MAGICAL_LEAF]  = _("HOJA MÁGICA"),
    [MOVE_WATER_SPORT]   = _("HIDROCHORRO"),
    [MOVE_CALM_MIND]     = _("PAZ MENTAL"),
    [MOVE_LEAF_BLADE]    = _("HOJA NAVAJA"),
    [MOVE_DRAGON_DANCE]  = _("DANZA DRAGÓN"),
    [MOVE_ROCK_BLAST]    = _("PEDRADA"),
    [MOVE_SHOCK_WAVE]    = _("ONDA CHOQUE"),
    [MOVE_WATER_PULSE]   = _("PULSO AGUA"),
    [MOVE_DOOM_DESIRE]   = _("DESEO FINAL"),
    [MOVE_PSYCHO_BOOST]  = _("PSICOIMPULSO")
};

const u8 gLongMoveNames[][19] = {
    [MOVE_NONE]          = _("-$$$$$$"),
    [MOVE_POUND]         = _("GOLPE"),
    [MOVE_KARATE_CHOP]   = _("GOLPE KÁRATE"),
    [MOVE_DOUBLE_SLAP]   = _("BOFETADAS"),
    [MOVE_COMET_PUNCH]   = _("PUÑO COMETA"),
    [MOVE_MEGA_PUNCH]    = _("MEGA PUÑO"),
    [MOVE_PAY_DAY]       = _("DÍA DE PAGO"),
    [MOVE_FIRE_PUNCH]    = _("PUÑO DE FUEGO"),
    [MOVE_ICE_PUNCH]     = _("PUÑO DE HIELO"),
    [MOVE_THUNDER_PUNCH] = _("PUÑO DE TRUENO"),
    [MOVE_SCRATCH]       = _("ARAÑAZO"),
    [MOVE_VICE_GRIP]     = _("FUERZA DE GARRA"),
    [MOVE_GUILLOTINE]    = _("GUILLOTINA"),
    [MOVE_RAZOR_WIND]    = _("VIENTO CORTANTE"),
    [MOVE_SWORDS_DANCE]  = _("DANZA DE ESPADAS"),
    [MOVE_CUT]           = _("CORTE"),
    [MOVE_GUST]          = _("RÁFAGA"),
    [MOVE_WING_ATTACK]   = _("ATAQUE DE ALA"),
    [MOVE_WHIRLWIND]     = _("TORBELLINO"),
    [MOVE_FLY]           = _("VUELO"),
    [MOVE_BIND]          = _("ATADURA"),
    [MOVE_SLAM]          = _("GOLPAZO"),
    [MOVE_VINE_WHIP]     = _("LÁTIGO CEPA"),
    [MOVE_STOMP]         = _("PISOTÓN"),
    [MOVE_DOUBLE_KICK]   = _("PATADA DOBLE"),
    [MOVE_MEGA_KICK]     = _("MEGA PATADA"),
    [MOVE_JUMP_KICK]     = _("PATADA DE SALTO"),
    [MOVE_ROLLING_KICK]  = _("PATADA GIRO"),
    [MOVE_SAND_ATTACK]   = _("ATAQUE ARENA"),
    [MOVE_HEADBUTT]      = _("GOLPE CABEZA"),
    [MOVE_HORN_ATTACK]   = _("CORNADA"),
    [MOVE_FURY_ATTACK]   = _("ATAQUE FURIA"),
    [MOVE_HORN_DRILL]    = _("PERFORADOR"),
    [MOVE_TACKLE]        = _("TACLEADA"),
    [MOVE_BODY_SLAM]     = _("GOLPE CUERPO"),
    [MOVE_WRAP]          = _("ENVOLTURA"),
    [MOVE_TAKE_DOWN]     = _("DERRIBO"),
    [MOVE_THRASH]        = _("PALIZA"),
    [MOVE_DOUBLE_EDGE]   = _("DOBLE FILO"),
    [MOVE_TAIL_WHIP]     = _("MENEO DE COLA"),
    [MOVE_POISON_STING]  = _("PIQUETE VENENOSO"),
    [MOVE_TWINEEDLE]     = _("PIQUETE DOBLE"),
    [MOVE_PIN_MISSILE]   = _("MISILES AGUJA"),
    [MOVE_LEER]          = _("MIRADA"),
    [MOVE_BITE]          = _("MORDIDA"),
    [MOVE_GROWL]         = _("GRUÑIDO"),
    [MOVE_ROAR]          = _("RUGIDO"),
    [MOVE_SING]          = _("CANTO"),
    [MOVE_SUPERSONIC]    = _("SUPERSÓNICO"),
    [MOVE_SONIC_BOOM]    = _("EXPLOSIÓN SÓNICA"),
    [MOVE_DISABLE]       = _("ANULACIÓN"),
    [MOVE_ACID]          = _("ÁCIDO"),
    [MOVE_EMBER]         = _("BRASAS"),
    [MOVE_FLAMETHROWER]  = _("LANZALLAMAS"),
    [MOVE_MIST]          = _("NIEBLA"),
    [MOVE_WATER_GUN]     = _("CHORRO DE AGUA"),
    [MOVE_HYDRO_PUMP]    = _("HIDROBOMBA"),
    [MOVE_SURF]          = _("SURF"),
    [MOVE_ICE_BEAM]      = _("RAYO HIELO"),
    [MOVE_BLIZZARD]      = _("VENTISCA"),
    [MOVE_PSYBEAM]       = _("PSICORRAYO"),
    [MOVE_BUBBLE_BEAM]   = _("RAYO BURBUJA"),
    [MOVE_AURORA_BEAM]   = _("RAYO AURORA"),
    [MOVE_HYPER_BEAM]    = _("HIPERRAYO"),
    [MOVE_PECK]          = _("PICOTAZO"),
    [MOVE_DRILL_PECK]    = _("PICO TALADRO"),
    [MOVE_SUBMISSION]    = _("SUMISIÓN"),
    [MOVE_LOW_KICK]      = _("PATADA BAJA"),
    [MOVE_COUNTER]       = _("CONTRAATAQUE"),
    [MOVE_SEISMIC_TOSS]  = _("TIRO SÍSMICO"),
    [MOVE_STRENGTH]      = _("FUERZA"),
    [MOVE_ABSORB]        = _("ABSORCIÓN"),
    [MOVE_MEGA_DRAIN]    = _("MEGA DRENADO"),
    [MOVE_LEECH_SEED]    = _("DRENADORAS"),
    [MOVE_GROWTH]        = _("CRECIMIENTO"),
    [MOVE_RAZOR_LEAF]    = _("HOJA AFILADA"),
    [MOVE_SOLAR_BEAM]    = _("RAYO SOLAR"),
    [MOVE_POISON_POWDER] = _("POLVO VENENO"),
    [MOVE_STUN_SPORE]    = _("POLVO PARÁLISIS"),
    [MOVE_SLEEP_POWDER]  = _("POLVO SUEÑO"),
    [MOVE_PETAL_DANCE]   = _("DANZA PÉTALO"),
    [MOVE_STRING_SHOT]   = _("DISPARO SEDA"),
    [MOVE_DRAGON_RAGE]   = _("FURIA DRAGÓN"),
    [MOVE_FIRE_SPIN]     = _("GIRO FUEGO"),
    [MOVE_THUNDER_SHOCK] = _("IMPACTRUENO"),
    [MOVE_THUNDERBOLT]   = _("ATACTRUENO"),
    [MOVE_THUNDER_WAVE]  = _("ONDA TRUENO"),
    [MOVE_THUNDER]       = _("TRUENO"),
    [MOVE_ROCK_THROW]    = _("LANZA ROCAS"),
    [MOVE_EARTHQUAKE]    = _("TERREMOTO"),
    [MOVE_FISSURE]       = _("FISURA"),
    [MOVE_DIG]           = _("CAVAR"),
    [MOVE_TOXIC]         = _("TÓXICO"),
    [MOVE_CONFUSION]     = _("CONFUSIÓN"),
    [MOVE_PSYCHIC]       = _("PSÍQUICO"),
    [MOVE_HYPNOSIS]      = _("HIPNOSIS"),
    [MOVE_MEDITATE]      = _("MEDITAR"),
    [MOVE_AGILITY]       = _("AGILIDAD"),
    [MOVE_QUICK_ATTACK]  = _("ATAQUE RÁPIDO"),
    [MOVE_RAGE]          = _("FURIA"),
    [MOVE_TELEPORT]      = _("TELETRANSPORTE"),
    [MOVE_NIGHT_SHADE]   = _("TINIEBLAS"),
    [MOVE_MIMIC]         = _("MÍMICA"),
    [MOVE_SCREECH]       = _("ALARIDO"),
    [MOVE_DOUBLE_TEAM]   = _("DOBLE EQUIPO"),
    [MOVE_RECOVER]       = _("RECUPERACIÓN"),
    [MOVE_HARDEN]        = _("ENDURECER"),
    [MOVE_MINIMIZE]      = _("MINIMIZAR"),
    [MOVE_SMOKESCREEN]   = _("PANTALLA DE HUMO"),
    [MOVE_CONFUSE_RAY]   = _("RAYO DE CONFUSIÓN"),
    [MOVE_WITHDRAW]      = _("REFUGIO"),
    [MOVE_DEFENSE_CURL]  = _("RIZO DEFENSA"),
    [MOVE_BARRIER]       = _("BARRERA"),
    [MOVE_LIGHT_SCREEN]  = _("PANTALLA DE LUZ"),
    [MOVE_HAZE]          = _("NEBLINA"),
    [MOVE_REFLECT]       = _("REFLEJO"),
    [MOVE_FOCUS_ENERGY]  = _("FOCO ENERGÍA"),
    [MOVE_BIDE]          = _("PERSEVERAR"),
    [MOVE_METRONOME]     = _("METRÓNOMO"),
    [MOVE_MIRROR_MOVE]   = _("MOVIMIENTO ESPEJO"),
    [MOVE_SELF_DESTRUCT] = _("AUTODESTRUCCIÓN"),
    [MOVE_EGG_BOMB]      = _("BOMBA HUEVO"),
    [MOVE_LICK]          = _("LENGÜETAZO"),
    [MOVE_SMOG]          = _("HUMO"),
    [MOVE_SLUDGE]        = _("DESECHOS"),
    [MOVE_BONE_CLUB]     = _("GOLPE HUESO"),
    [MOVE_FIRE_BLAST]    = _("LLAMARADA"),
    [MOVE_WATERFALL]     = _("CASCADA"),
    [MOVE_CLAMP]         = _("SUJETAR"),
    [MOVE_SWIFT]         = _("VELOCIDAD"),
    [MOVE_SKULL_BASH]    = _("CABEZAZO"),
    [MOVE_SPIKE_CANNON]  = _("CAÑON PICOS"),
    [MOVE_CONSTRICT]     = _("APRETÓN"),
    [MOVE_AMNESIA]       = _("AMNESIA"),
    [MOVE_KINESIS]       = _("KINÉTICO"),
    [MOVE_SOFT_BOILED]   = _("HUEVO COCIDO"),
    [MOVE_HI_JUMP_KICK]  = _("PATADA SALTO ALTO"),
    [MOVE_GLARE]         = _("MIRADA DE REPTIL"),
    [MOVE_DREAM_EATER]   = _("COME SUEÑOS"),
    [MOVE_POISON_GAS]    = _("GAS VENENOSO"),
    [MOVE_BARRAGE]       = _("BOMBARDEO"),
    [MOVE_LEECH_LIFE]    = _("CHUPA VIDA"),
    [MOVE_LOVELY_KISS]   = _("BESO AMOROSO"),
    [MOVE_SKY_ATTACK]    = _("ATAQUE AÉREO"),
    [MOVE_TRANSFORM]     = _("TRANSFORMACIÓN"),
    [MOVE_BUBBLE]        = _("BURBUJA"),
    [MOVE_DIZZY_PUNCH]   = _("PUÑO MAREO"),
    [MOVE_SPORE]         = _("ESPORAS"),
    [MOVE_FLASH]         = _("DESTELLO"),
    [MOVE_PSYWAVE]       = _("PSICOONDA"),
    [MOVE_SPLASH]        = _("SALTO"),
    [MOVE_ACID_ARMOR]    = _("ARMADURA ÁCIDA"),
    [MOVE_CRABHAMMER]    = _("MARTILLAZO"),
    [MOVE_EXPLOSION]     = _("EXPLOSIÓN"),
    [MOVE_FURY_SWIPES]   = _("GARRAS FURIA"),
    [MOVE_BONEMERANG]    = _("HUESORÁN"),
    [MOVE_REST]          = _("DESCANSO"),
    [MOVE_ROCK_SLIDE]    = _("AVALANCHA"),
    [MOVE_HYPER_FANG]    = _("HIPER COLMILLO"),
    [MOVE_SHARPEN]       = _("AFILAR"),
    [MOVE_CONVERSION]    = _("CONVERSIÓN"),
    [MOVE_TRI_ATTACK]    = _("TRIATAQUE"),
    [MOVE_SUPER_FANG]    = _("SUPER COLMILLO"),
    [MOVE_SLASH]         = _("CUCHILLADA"),
    [MOVE_SUBSTITUTE]    = _("SUSTITUTO"),
    [MOVE_STRUGGLE]      = _("FORCEJEO"),
    [MOVE_SKETCH]        = _("BOCETO"),
    [MOVE_TRIPLE_KICK]   = _("PATADA TRIPLE"),
    [MOVE_THIEF]         = _("LADRÓN"),
    [MOVE_SPIDER_WEB]    = _("TELARAÑA"),
    [MOVE_MIND_READER]   = _("TELÉPATA"),
    [MOVE_NIGHTMARE]     = _("PESADILLA"),
    [MOVE_FLAME_WHEEL]   = _("RUEDA FUEGO"),
    [MOVE_SNORE]         = _("RONQUIDO"),
    [MOVE_CURSE]         = _("MALDICIÓN"),
    [MOVE_FLAIL]         = _("AZOTAR"),
    [MOVE_CONVERSION_2]  = _("CONVERSIÓN 2"),
    [MOVE_AEROBLAST]     = _("AEROCHORRO"),
    [MOVE_COTTON_SPORE]  = _("ESPORAGODÓN"),
    [MOVE_REVERSAL]      = _("INVERSIÓN"),
    [MOVE_SPITE]         = _("RENCOR"),
    [MOVE_POWDER_SNOW]   = _("POLVO NIEVE"),
    [MOVE_PROTECT]       = _("PROTECCIÓN"),
    [MOVE_MACH_PUNCH]    = _("PUÑO VELOZ"),
    [MOVE_SCARY_FACE]    = _("CARA MIEDO"),
    [MOVE_FAINT_ATTACK]  = _("ATAQUE FINTA"),
    [MOVE_SWEET_KISS]    = _("BESO DULCE"),
    [MOVE_BELLY_DRUM]    = _("TAMBOR"),
    [MOVE_SLUDGE_BOMB]   = _("BOMBA LODO"),
    [MOVE_MUD_SLAP]      = _("GOLPE LODO"),
    [MOVE_OCTAZOOKA]     = _("OCTAZOOKA"),
    [MOVE_SPIKES]        = _("PÚAS"),
    [MOVE_ZAP_CANNON]    = _("ELECTRO CAÑON"),
    [MOVE_FORESIGHT]     = _("VISIÓN"),
    [MOVE_DESTINY_BOND]  = _("LAZO DEL DESTINO"),
    [MOVE_PERISH_SONG]   = _("CANTO MORTAL"),
    [MOVE_ICY_WIND]      = _("VIENTO HIELO"),
    [MOVE_DETECT]        = _("DETECCIÓN"),
    [MOVE_BONE_RUSH]     = _("HUESO VELOZ"),
    [MOVE_LOCK_ON]       = _("FIJAR BLANCO"),
    [MOVE_OUTRAGE]       = _("ENFADO"),
    [MOVE_SANDSTORM]     = _("TORMENTA DE ARENA"),
    [MOVE_GIGA_DRAIN]    = _("GIGA DRENADO"),
    [MOVE_ENDURE]        = _("RESISTIR"),
    [MOVE_CHARM]         = _("ENCANTO"),
    [MOVE_ROLLOUT]       = _("RODADA"),
    [MOVE_FALSE_SWIPE]   = _("CORTE FALSO"),
    [MOVE_SWAGGER]       = _("DESCONTROL"),
    [MOVE_MILK_DRINK]    = _("BEBE LECHE"),
    [MOVE_SPARK]         = _("CHISPA"),
    [MOVE_FURY_CUTTER]   = _("CORTE FURIA"),
    [MOVE_STEEL_WING]    = _("ALA DE ACERO"),
    [MOVE_MEAN_LOOK]     = _("MIRADA FIJA"),
    [MOVE_ATTRACT]       = _("ATRACCIÓN"),
    [MOVE_SLEEP_TALK]    = _("SONÁMBULO"),
    [MOVE_HEAL_BELL]     = _("CAMPANA CURA"),
    [MOVE_RETURN]        = _("RETRIBUCIÓN"),
    [MOVE_PRESENT]       = _("OBSEQUIO"),
    [MOVE_FRUSTRATION]   = _("FRUSTRACIÓN"),
    [MOVE_SAFEGUARD]     = _("SALVAGUARDA"),
    [MOVE_PAIN_SPLIT]    = _("DIVIDE DOLOR"),
    [MOVE_SACRED_FIRE]   = _("FUEGO SAGRADO"),
    [MOVE_MAGNITUDE]     = _("MAGNITUD"),
    [MOVE_DYNAMIC_PUNCH] = _("PUÑO DINÁMICO"),
    [MOVE_MEGAHORN]      = _("MEGA CUERNO"),
    [MOVE_DRAGON_BREATH] = _("ALIENTO DRAGÓN"),
    [MOVE_BATON_PASS]    = _("RELEVO"),
    [MOVE_ENCORE]        = _("OTRA VEZ"),
    [MOVE_PURSUIT]       = _("PERSECUCIÓN"),
    [MOVE_RAPID_SPIN]    = _("GIRO RÁPIDO"),
    [MOVE_SWEET_SCENT]   = _("DULCE AROMA"),
    [MOVE_IRON_TAIL]     = _("COLA DE HIERRO"),
    [MOVE_METAL_CLAW]    = _("GARRA METAL"),
    [MOVE_VITAL_THROW]   = _("TIRO VITAL"),
    [MOVE_MORNING_SUN]   = _("SOL MATUTINO"),
    [MOVE_SYNTHESIS]     = _("SÍNTESIS"),
    [MOVE_MOONLIGHT]     = _("LUZ LUNAR"),
    [MOVE_HIDDEN_POWER]  = _("PODER OCULTO"),
    [MOVE_CROSS_CHOP]    = _("TAJO CRUZADO"),
    [MOVE_TWISTER]       = _("CICLÓN"),
    [MOVE_RAIN_DANCE]    = _("DANZA LLUVIA"),
    [MOVE_SUNNY_DAY]     = _("DÍA SOLEADO"),
    [MOVE_CRUNCH]        = _("TRITURAR"),
    [MOVE_MIRROR_COAT]   = _("MANTO ESPEJO"),
    [MOVE_PSYCH_UP]      = _("MÁS PSIQUE"),
    [MOVE_EXTREME_SPEED] = _("VELOCIDAD EXTREMA"),
    [MOVE_ANCIENT_POWER] = _("PODER PASADO"),
    [MOVE_SHADOW_BALL]   = _("BOLA SOMBRA"),
    [MOVE_FUTURE_SIGHT]  = _("VISTA FUTURA"),
    [MOVE_ROCK_SMASH]    = _("ROMPE ROCAS"),
    [MOVE_WHIRLPOOL]     = _("REMOLINO"),
    [MOVE_BEAT_UP]       = _("ATAQUE GRUPAL"),
    [MOVE_FAKE_OUT]      = _("SORPRESA"),
    [MOVE_UPROAR]        = _("ALBOROTO"),
    [MOVE_STOCKPILE]     = _("RESERVAS"),
    [MOVE_SPIT_UP]       = _("ESCUPIDA"),
    [MOVE_SWALLOW]       = _("TRAGAR"),
    [MOVE_HEAT_WAVE]     = _("ONDA DE CALOR"),
    [MOVE_HAIL]          = _("GRANIZO"),
    [MOVE_TORMENT]       = _("TORMENTO"),
    [MOVE_FLATTER]       = _("HALAGO"),
    [MOVE_WILL_O_WISP]   = _("FUEGO FAUTO"),
    [MOVE_MEMENTO]       = _("LEGADO"),
    [MOVE_FACADE]        = _("FACHADA"),
    [MOVE_FOCUS_PUNCH]   = _("PUÑO CERTERO"),
    [MOVE_SMELLING_SALT] = _("ESTÍMULO"),
    [MOVE_FOLLOW_ME]     = _("SEÑUELO"),
    [MOVE_NATURE_POWER]  = _("ADAPTACIÓN"),
    [MOVE_CHARGE]        = _("CARGA"),
    [MOVE_TAUNT]         = _("MOFA"),
    [MOVE_HELPING_HAND]  = _("REFUERZO"),
    [MOVE_TRICK]         = _("TRUCO"),
    [MOVE_ROLE_PLAY]     = _("IMITACIÓN"),
    [MOVE_WISH]          = _("DESEO"),
    [MOVE_ASSIST]        = _("AYUDA"),
    [MOVE_INGRAIN]       = _("ARRAIGO"),
    [MOVE_SUPERPOWER]    = _("SUPERPODER"),
    [MOVE_MAGIC_COAT]    = _("CAPA MÁGICA"),
    [MOVE_RECYCLE]       = _("RECICLAJE"),
    [MOVE_REVENGE]       = _("VENGANZA"),
    [MOVE_BRICK_BREAK]   = _("KARATAZO"),
    [MOVE_YAWN]          = _("BOSTEZO"),
    [MOVE_KNOCK_OFF]     = _("DESARME"),
    [MOVE_ENDEAVOR]      = _("ESFUERZO"),
    [MOVE_ERUPTION]      = _("ERUPCIÓN"),
    [MOVE_SKILL_SWAP]    = _("INTERCAMBIO"),
    [MOVE_IMPRISON]      = _("SELLAR"),
    [MOVE_REFRESH]       = _("RENOVAR"),
    [MOVE_GRUDGE]        = _("RABIA"),
    [MOVE_SNATCH]        = _("ROBO"),
    [MOVE_SECRET_POWER]  = _("PODER OCULTO"),
    [MOVE_DIVE]          = _("BUCEO"),
    [MOVE_ARM_THRUST]    = _("BRAZADA"),
    [MOVE_CAMOUFLAGE]    = _("CAMUFLAJE"),
    [MOVE_TAIL_GLOW]     = _("COLA DE LUZ"),
    [MOVE_LUSTER_PURGE]  = _("RESPLANDOR"),
    [MOVE_MIST_BALL]     = _("BOLA NIEBLA"),
    [MOVE_FEATHER_DANCE] = _("DANZA PLUMAS"),
    [MOVE_TEETER_DANCE]  = _("DANZA CAOS"),
    [MOVE_BLAZE_KICK]    = _("PATADA FUEGO"),
    [MOVE_MUD_SPORT]     = _("CHAPOTEO LODO"),
    [MOVE_ICE_BALL]      = _("BOLA HIELO"),
    [MOVE_NEEDLE_ARM]    = _("BRAZO AGUJA"),
    [MOVE_SLACK_OFF]     = _("RELAJAR"),
    [MOVE_HYPER_VOICE]   = _("HIPER VOZ"),
    [MOVE_POISON_FANG]   = _("COLMILLO VENENOSO"),
    [MOVE_CRUSH_CLAW]    = _("GARRA BRUTAL"),
    [MOVE_BLAST_BURN]    = _("ANILLO FUEGO"),
    [MOVE_HYDRO_CANNON]  = _("HIDROCAÑON"),
    [MOVE_METEOR_MASH]   = _("PUÑO METEORO"),
    [MOVE_ASTONISH]      = _("IMPRESIÓN"),
    [MOVE_WEATHER_BALL]  = _("METEOROBOLA"),
    [MOVE_AROMATHERAPY]  = _("AROMATERAPIA"),
    [MOVE_FAKE_TEARS]    = _("LLANTO FALSO"),
    [MOVE_AIR_CUTTER]    = _("AIRE AFILADO"),
    [MOVE_OVERHEAT]      = _("SOFOCO"),
    [MOVE_ODOR_SLEUTH]   = _("RASTREO"),
    [MOVE_ROCK_TOMB]     = _("TUMBA ROCAS"),
    [MOVE_SILVER_WIND]   = _("VIENTO PLATA"),
    [MOVE_METAL_SOUND]   = _("ECO METÁLICO"),
    [MOVE_GRASS_WHISTLE] = _("SONIDO HOJA"),
    [MOVE_TICKLE]        = _("COSQUILLAS"),
    [MOVE_COSMIC_POWER]  = _("MASA CÓSMICA"),
    [MOVE_WATER_SPOUT]   = _("SALPICAR"),
    [MOVE_SIGNAL_BEAM]   = _("RAYO SEÑAL"),
    [MOVE_SHADOW_PUNCH]  = _("PUÑO SOMBRA"),
    [MOVE_EXTRASENSORY]  = _("PARANORMAL"),
    [MOVE_SKY_UPPERCUT]  = _("GANCHO ALTO"),
    [MOVE_SAND_TOMB]     = _("TUMBA DE ARENA"),
    [MOVE_SHEER_COLD]    = _("FRÍO EXTREMO"),
    [MOVE_MUDDY_WATER]   = _("AGUA LODOSA"),
    [MOVE_BULLET_SEED]   = _("BALAS SEMILLA"),
    [MOVE_AERIAL_ACE]    = _("AS AÉREO"),
    [MOVE_ICICLE_SPEAR]  = _("CARÁMBANOS"),
    [MOVE_IRON_DEFENSE]  = _("DEFENSA FÉRREA"),
    [MOVE_BLOCK]         = _("BLOQUEO"),
    [MOVE_HOWL]          = _("AULLIDO"),
    [MOVE_DRAGON_CLAW]   = _("GARRA DRAGÓN"),
    [MOVE_FRENZY_PLANT]  = _("PLANTA FEROZ"),
    [MOVE_BULK_UP]       = _("CORPULENCIA"),
    [MOVE_BOUNCE]        = _("REBOTE"),
    [MOVE_MUD_SHOT]      = _("DISPARO LODO"),
    [MOVE_POISON_TAIL]   = _("COLA VENENO"),
    [MOVE_COVET]         = _("ANTOJO"),
    [MOVE_VOLT_TACKLE]   = _("TACLEADA VOLTIOS"),
    [MOVE_MAGICAL_LEAF]  = _("HOJA MÁGICA"),
    [MOVE_WATER_SPORT]   = _("HIDROCHORRO"),
    [MOVE_CALM_MIND]     = _("PAZ MENTAL"),
    [MOVE_LEAF_BLADE]    = _("HOJA NAVAJA"),
    [MOVE_DRAGON_DANCE]  = _("DANZA DRAGÓN"),
    [MOVE_ROCK_BLAST]    = _("PEDRADA"),
    [MOVE_SHOCK_WAVE]    = _("ONDA CHOQUE"),
    [MOVE_WATER_PULSE]   = _("PULSO AGUA"),
    [MOVE_DOOM_DESIRE]   = _("DESEO FINAL"),
    [MOVE_PSYCHO_BOOST]  = _("PSICOIMPULSO")
};

