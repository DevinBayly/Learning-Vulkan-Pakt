

// *******************
// VERTEX BUFFER DATA:
// *******************

#ifdef DEFINITION
struct vertex
{
	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3	color;
	glm::vec2	texCoord;
};
#endif
// this is the big list of triangles, this is used if we don't ask for there to be indexing


struct vertex VertexData[ ] = {{{1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.48625913495382356, 0.33368803473511033, 0.2662388604051329}, {0, 0}}, {{1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {0.2950268864697979, 0.3977583215913869, 0.9068971381178405}, {0, 0}}, {{1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.734092926132951, 0.17872264009947836, 0.6757431141910277}, {0, 0}}, {{1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.48625913495382356, 0.33368803473511033, 0.2662388604051329}, {0, 0}}, {{1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.734092926132951, 0.17872264009947836, 0.6757431141910277}, {0, 0}}, {{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.03418814584301599, 0.29532046059343897, 0.920212858180491}, {0, 0}}, {{0.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {0.4745391534577964, 0.8161225654576675, 0.6478984355007619}, {0, 0}}, {{0.0, 0.0, 1.0}, {-1.0, 0.0, 0.0}, {0.01294210983416988, 0.9847317428080169, 0.4992061659268858}, {0, 0}}, {{0.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {0.9252266701204693, 0.010525131047866676, 0.921033336820219}, {0, 0}}, {{0.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {0.4745391534577964, 0.8161225654576675, 0.6478984355007619}, {0, 0}}, {{0.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {0.9252266701204693, 0.010525131047866676, 0.921033336820219}, {0, 0}}, {{0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}, {0.809588782567649, 0.5512348179419305, 0.03194633140648584}, {0, 0}}, {{1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {0.5134807383907298, 0.2765459151056533, 0.9158890153388549}, {0, 0}}, {{1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.543372798550588, 0.7074701318148922, 0.9993841685365563}, {0, 0}}, {{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.7539035197334361, 0.2703207443975828, 0.1825837228091851}, {0, 0}}, {{1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {0.5134807383907298, 0.2765459151056533, 0.9158890153388549}, {0, 0}}, {{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.7539035197334361, 0.2703207443975828, 0.1825837228091851}, {0, 0}}, {{0.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {0.233516522392747, 0.2941992442814364, 0.8656871515051178}, {0, 0}}, {{1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.5530214176612677, 0.4893872368996356, 0.37567431712505706}, {0, 0}}, {{1.0, 0.0, 1.0}, {0.0, -1.0, 0.0}, {0.4719057767257625, 0.8687975386586637, 0.8685115630511935}, {0, 0}}, {{0.0, 0.0, 1.0}, {0.0, -1.0, 0.0}, {0.4011125294693364, 0.710398181418143, 0.93496390709058}, {0, 0}}, {{1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.5530214176612677, 0.4893872368996356, 0.37567431712505706}, {0, 0}}, {{0.0, 0.0, 1.0}, {0.0, -1.0, 0.0}, {0.4011125294693364, 0.710398181418143, 0.93496390709058}, {0, 0}}, {{0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.16404804467099077, 0.6000498597687959, 0.8264358884892458}, {0, 0}}, {{1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.7315490111937845, 0.6162561305986086, 0.18524431205328862}, {0, 0}}, {{1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.49740747735217317, 0.47840303448155763, 0.8028197499923349}, {0, 0}}, {{0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.1995742748655056, 0.5552031388026624, 0.41650184724436035}, {0, 0}}, {{1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.7315490111937845, 0.6162561305986086, 0.18524431205328862}, {0, 0}}, {{0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.1995742748655056, 0.5552031388026624, 0.41650184724436035}, {0, 0}}, {{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.981567307391299, 0.07261069630625605, 0.9432908236802854}, {0, 0}}, {{1.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {0.1819188721967846, 0.9648128976400843, 0.8198319200546079}, {0, 0}}, {{1.0, 0.0, 0.0}, {0.0, 0.0, -1.0}, {0.9789783787056978, 0.6259714168940033, 0.2095221462325655}, {0, 0}}, {{0.0, 0.0, 0.0}, {0.0, 0.0, -1.0}, {0.6740760330807186, 0.9874700897745531, 0.8894856323959089}, {0, 0}}, {{1.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {0.1819188721967846, 0.9648128976400843, 0.8198319200546079}, {0, 0}}, {{0.0, 0.0, 0.0}, {0.0, 0.0, -1.0}, {0.6740760330807186, 0.9874700897745531, 0.8894856323959089}, {0, 0}}, {{0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {0.31008595582267284, 0.8052249169225127, 0.42839243044616593}, {0, 0}}};

struct vertex JustVertexData[ ] = {{{-1.0, 1.0, -1.0}, {-0.0, 1.0, -0.0}, {0.6190838513750706, 0.983502600557997, 0.184754330485024}, {0.5181270241737366, 0.5552859902381897}}, {{1.0, 1.0, 1.0}, {-0.0, 1.0, -0.0}, {0.277921890625758, 0.26749327973792114, 0.6695154058373357}, {0.4781309962272644, 0.512719988822937}}, {{1.0, 1.0, -1.0}, {-0.0, 1.0, -0.0}, {0.6150838862453376, 0.8455404721866432, 0.540082869241724}, {0.5532000064849854, 0.5279240012168884}}, {{1.0, 1.0, 1.0}, {-0.0, -0.0, 1.0}, {0.6744429905636388, 0.4306330214148144, 0.2320781057417386}, {0.4781309962272644, 0.512719988822937}}, {{-1.0, -1.0, 1.0}, {-0.0, -0.0, 1.0}, {0.6409797162450811, 0.6220450443495725, 0.5267515074062664}, {0.45188599824905396, 0.4747450053691864}}, {{1.0, -1.0, 1.0}, {-0.0, -0.0, 1.0}, {0.5347448930570684, 0.472272156303705, 0.5638578289272369}, {0.4803290069103241, 0.44000500440597534}}, {{-1.0, 1.0, 1.0}, {-1.0, -0.0, -0.0}, {0.7527887422117188, 0.444868763962711, 0.8332562046605501}, {0.4484789967536926, 0.5425639748573303}}, {{-1.0, -1.0, -1.0}, {-1.0, -0.0, -0.0}, {0.741909770484441, 0.14557166028453916, 0.5708130526335332}, {0.5182420015335083, 0.48938998579978943}}, {{-1.0, -1.0, 1.0}, {-1.0, -0.0, -0.0}, {0.7877314494897817, 0.5112133924250472, 0.6756620983778765}, {0.45188599824905396, 0.4747450053691864}}, {{1.0, -1.0, -1.0}, {-0.0, -1.0, -0.0}, {0.8174643598312282, 0.5014726544087788, 0.12222617743570652}, {0.5515859723091125, 0.4573819935321808}}, {{-1.0, -1.0, 1.0}, {-0.0, -1.0, -0.0}, {0.9811480828009636, 0.6947971948163887, 0.9308586112100445}, {0.45188599824905396, 0.4747450053691864}}, {{-1.0, -1.0, -1.0}, {-0.0, -1.0, -0.0}, {0.5130575950345109, 0.4931130059613438, 0.33750313891658723}, {0.5182420015335083, 0.48938998579978943}}, {{1.0, 1.0, -1.0}, {1.0, -0.0, -0.0}, {0.8726682225110824, 0.2691924111640457, 0.4746639994596018}, {0.5532000064849854, 0.5279240012168884}}, {{1.0, -1.0, 1.0}, {1.0, -0.0, -0.0}, {0.907233742212988, 0.4482898172229184, 0.26698125364072245}, {0.4803290069103241, 0.44000500440597534}}, {{1.0, -1.0, -1.0}, {1.0, -0.0, -0.0}, {0.38845761666379985, 0.09941243355714147, 0.5524250854216016}, {0.5515859723091125, 0.4573819935321808}}, {{-1.0, 1.0, -1.0}, {-0.0, -0.0, -1.0}, {0.2861102250737505, 0.8117823265886338, 0.831008912535692}, {0.5181270241737366, 0.5552859902381897}}, {{1.0, -1.0, -1.0}, {-0.0, -0.0, -1.0}, {0.7088548970064877, 0.462018339086666, 0.8322579993972398}, {0.5515859723091125, 0.4573819935321808}}, {{-1.0, -1.0, -1.0}, {-0.0, -0.0, -1.0}, {0.6389523887137655, 0.01811986315685854, 0.3845309203175822}, {0.5182420015335083, 0.48938998579978943}}, {{-1.0, 1.0, 1.0}, {-0.0, 1.0, -0.0}, {0.6392995796857731, 0.7943608244758168, 0.3216546948842828}, {0.5181270241737366, 0.5552859902381897}}, {{-1.0, 1.0, 1.0}, {-0.0, -0.0, 1.0}, {0.9120746394309973, 0.11354086865316859, 0.7185022222157552}, {0.4484789967536926, 0.5425639748573303}}, {{-1.0, 1.0, -1.0}, {-1.0, -0.0, -0.0}, {0.7927824289431211, 0.7604733136610686, 0.6683750070860082}, {0.4781309962272644, 0.512719988822937}}, {{1.0, -1.0, 1.0}, {-0.0, -1.0, -0.0}, {0.9877825036217075, 0.2755922769336189, 0.05348894713466368}, {0.4781309962272644, 0.512719988822937}}, {{1.0, 1.0, 1.0}, {1.0, -0.0, -0.0}, {0.03805711511422705, 0.038598839953339126, 0.06410591192358783}, {0.4484789967536926, 0.5425639748573303}}, {{1.0, 1.0, -1.0}, {-0.0, -0.0, -1.0}, {0.9243174499045088, 0.8745435534144448, 0.9827862527264499}, {0.45188599824905396, 0.4747450053691864}}} ;


int JustIndexData[ ] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16} ;