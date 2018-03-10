#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "../zad1/address_book.h"

void check_times(clock_t *real, struct rusage *usage, FILE *file);

int main() {
    time_t tt;
    srand((unsigned int) time(&tt));

    char *names[] = {"Hollee", "Rana", "James", "Winter", "Steven", "Adrienne", "Teegan", "Isaiah", "Olympia", "Ethan",
                     "Cole", "Brock", "Kerry", "Kuame", "Heidi", "Brenda", "Maile", "Joseph", "Davis", "Colt", "Tyrone",
                     "Amela", "Beverly", "Astra", "Ainsley", "Uriel", "Jocelyn", "Sheila", "Irene", "Claire", "Armando",
                     "Ivy", "Amelia", "Macaulay", "Erin", "Brooke", "Jessamine", "Dorothy", "Macon", "Miriam",
                     "Clementine", "Steel", "William", "Ignacia", "Robin", "Reese", "Charity", "Branden", "Price",
                     "Jelani", "Lenore", "Lavinia", "Echo", "Jarrod", "Florence", "Wyatt", "Dahlia", "Lucy", "Gisela",
                     "Amir", "Germaine", "Declan", "Ifeoma", "Heather", "Shad", "Kennedy", "Brady", "Bethany",
                     "Chester", "Althea", "Bethany", "Addison", "Jordan", "Barclay", "Glenna", "Maris", "Summer",
                     "Jakeem", "Octavia", "Lila", "Lucy", "Hiram", "Stephen", "Cyrus", "Regan", "Ella", "Nigel",
                     "August", "Dolan", "Fiona", "Emily", "Lara", "Nissim", "Sara", "Darius", "Zachery", "Ayanna",
                     "Blaze", "Nero", "Rowanname"}; //100
    char *surnames[] = {"Palmer", "Bean", "Delgado", "Gilbert", "Davenport", "Meyer", "Mcneil", "Sexton", "Russell",
                        "Duke", "Sosa", "Cobb", "Chapman", "Brennan", "Delgado", "Coffey", "Faulkner", "Fuller",
                        "Hansen", "Kelley", "Rogers", "Dale", "Harper", "Mccarty", "Cannon", "Salinas", "Cline",
                        "Bailey", "Stephenson", "Larsen", "Madden", "George", "Mcpherson", "Bradshaw", "Hayden", "Leon",
                        "Sykes", "Wade", "Barnes", "Lowe", "Sawyer", "Cannon", "Barber", "Cochran", "Benson", "Boone",
                        "Herman", "Fisher", "Mcbride", "Winters", "Dyer", "Church", "Shepherd", "Short", "Miranda",
                        "Chen", "Daniel", "Reilly", "Cobb", "Cooper", "Howard", "Cardenas", "Combs", "Holman", "Tyson",
                        "Dunlap", "Juarez", "Dean", "Blankenship", "Johnston", "Banks", "Monroe", "Green", "Downs",
                        "Mejia", "Cherry", "Scott", "Ingram", "Cardenas", "Forbes", "Herman", "Nunez", "Mckee", "Yates",
                        "Underwood", "Jefferson", "Barnett", "Raymond", "Hardin", "Tanner", "Bright", "Pena", "West",
                        "Cunningham", "Blevins", "Craft", "Carney", "Lowery", "Meyer", "Hawkins"}; //100
    char *birthdays[] = {"1991-10-16", "1996-05-24", "1940-03-17", "1989-05-09", "1957-02-18", "1967-01-20",
                         "1956-12-16", "1967-01-13", "1976-11-29", "1990-05-30", "1986-04-23", "1940-08-17",
                         "1947-02-09", "1992-07-10", "1983-01-21", "1988-09-29", "1943-12-12", "1967-04-21",
                         "1992-10-25", "1982-11-27", "1976-10-18", "1964-04-04", "1990-05-07", "1940-04-01",
                         "1954-11-24", "1981-01-21", "1989-03-11", "1959-01-10", "1950-03-16", "1988-05-11",
                         "1967-03-11", "1988-09-06", "1946-11-17", "1955-09-27", "1945-10-04", "1978-02-05",
                         "1976-11-06", "1946-10-31", "1941-06-24", "1975-04-27", "1978-05-15", "1959-04-01",
                         "1962-09-20", "1960-10-23", "1994-07-05", "1948-09-11", "1972-05-03", "1971-11-27",
                         "1983-08-18", "1961-07-09", "1958-06-18", "1976-02-09", "1949-06-23", "1981-10-24",
                         "1989-01-08", "1973-06-28", "1960-04-16", "1961-02-15", "1969-05-14", "1939-05-23",
                         "1953-06-25", "1957-12-12", "1964-02-10", "1949-10-29", "1957-07-14", "1967-01-10",
                         "1990-07-22", "1945-08-20", "1961-06-30", "1943-03-20", "1974-10-14", "1967-10-15",
                         "1985-02-03", "1961-10-10", "1961-10-31", "1986-04-16", "1973-01-17", "1942-06-04",
                         "1946-12-18", "1958-02-05", "1980-01-13", "1961-12-23", "1940-10-28", "1976-04-13",
                         "1966-10-30", "1975-07-19", "1961-08-30", "1953-10-14", "1948-01-23", "1962-01-25",
                         "1975-09-22", "1951-03-22", "1968-05-28", "1976-10-30", "1971-08-29", "1941-06-24",
                         "1996-09-28", "1986-10-06", "1951-10-22", "1996-04-18"};
    char *emails[] = {"Donec.elementum.lorem@liberoProinsed.edu", "ut.lacus@Aliquamultricesiaculis.net",
                      "pellentesque.massa.lobortis@anteNunc.net", "Nam.ac@quam.net", "ac@habitant.edu",
                      "commodo.tincidunt.nibh@sagittislobortismauris.com", "elit.pharetra.ut@tincidunt.edu",
                      "eleifend.non.dapibus@nibh.net", "adipiscing.non.luctus@ipsumSuspendisse.net",
                      "vulputate.ullamcorper.magna@enimCurabiturmassa.org", "hendrerit.a.arcu@CrasinterdumNunc.net",
                      "ipsum@incursus.co.uk", "tellus.non@nulla.ca", "id@Aliquam.net",
                      "erat.eget@SuspendissesagittisNullam.ca", "elit.pellentesque@accumsan.ca",
                      "elit.Etiam@tellus.org", "nulla.at.sem@libero.net", "pede.Cras.vulputate@molestietellus.org",
                      "Integer.eu.lacus@Pellentesque.co.uk", "Quisque.varius.Nam@magnaaneque.org",
                      "luctus@euaugue.co.uk", "erat.Vivamus@enimconsequatpurus.co.uk", "amet@dolordolor.edu",
                      "aliquam.adipiscing@Integereu.ca", "cursus.luctus@enim.co.uk", "faucibus.id.libero@acmi.org",
                      "ipsum.cursus@auctorquis.co.uk", "nunc.sed@lectusrutrum.org",
                      "mus.Donec.dignissim@Crassedleo.com", "Proin.dolor@lobortistellus.org",
                      "purus.Maecenas.libero@DuisgravidaPraesent.org", "vestibulum@Craspellentesque.net",
                      "feugiat.placerat.velit@rutrumeu.org", "Proin@ut.com", "quis@ultriciessem.ca",
                      "sapien@sitamet.net", "cursus.purus@etrutrumnon.com", "nunc@cursusIntegermollis.org",
                      "auctor@semper.com", "non@sed.ca", "amet.faucibus@aliquameros.ca", "Aenean@uterosnon.net",
                      "sit.amet@laoreetipsumCurabitur.net", "ipsum@aliquetmagna.co.uk",
                      "Nullam.enim.Sed@adipiscingelit.ca", "mi@ipsumsodales.ca", "Morbi@tellussem.ca",
                      "Nunc.mauris@Maurisquisturpis.org", "tellus.lorem@vel.net", "vel.venenatis.vel@sed.edu",
                      "cursus@blanditcongue.com", "auctor.vitae.aliquet@Nuncsollicitudin.ca",
                      "penatibus.et@odiovelest.org", "Duis.cursus.diam@etliberoProin.org",
                      "vitae.odio.sagittis@euismod.ca", "nisi@necorciDonec.co.uk", "in@nullavulputatedui.net",
                      "velit.eu.sem@famesac.org", "Donec.est.Nunc@nislelementum.net",
                      "arcu.Morbi.sit@blanditviverraDonec.co.uk", "penatibus.et.magnis@maurisSuspendisse.ca",
                      "sit@infaucibusorci.co.uk", "quis@Duis.org", "scelerisque.scelerisque.dui@diamSed.com",
                      "in@sagittis.com", "Nam.ligula.elit@interdumligula.com", "nibh.Quisque@accumsansedfacilisis.edu",
                      "ultrices.posuere@mattisInteger.org", "Donec@mollis.co.uk", "lorem.ipsum.sodales@aauctor.edu",
                      "arcu.Vestibulum@arcuMorbisit.org", "eu@elitelitfermentum.com", "nibh.dolor.nonummy@id.ca",
                      "eu.erat.semper@sedhendrerita.org", "erat.volutpat.Nulla@elitpretium.org",
                      "tempus.risus@ametornarelectus.edu", "lacus.Etiam@massa.com", "sed.pede.Cum@sitametrisus.ca",
                      "ut.cursus@dolorsitamet.ca", "mauris.sit@quispede.org", "amet@pellentesque.com",
                      "dolor.egestas.rhoncus@sagittisNullamvitae.edu", "non@mattissemper.net",
                      "in.consequat.enim@Donec.edu", "nibh.Phasellus.nulla@atarcu.net", "augue.ut@estarcuac.edu",
                      "sed.turpis.nec@nonummyFuscefermentum.edu", "ac.metus@arcuCurabiturut.org",
                      "nunc@eleifendnon.edu", "parturient.montes@ornarelectus.ca", "magna.a.neque@a.com",
                      "ipsum.cursus.vestibulum@Sedauctorodio.ca", "quis@Nunc.com", "Maecenas.libero@facilisis.co.uk",
                      "dolor@eueros.edu", "Duis.dignissim@varius.co.uk", "et.rutrum.eu@Suspendissesagittis.org",
                      "senectus@enimnislelementum.com", "porttitor.eros.nec@semperNam.com"};
    char *phones[] = {"1-387-449-6802", "1-231-870-0271", "1-211-134-0652", "1-761-125-3853", "1-143-822-3263",
                      "1-631-941-7117", "1-987-251-5406", "1-643-346-5740", "1-464-115-2887", "1-563-720-3418",
                      "1-409-761-8896", "1-496-144-7058", "1-911-329-2495", "1-612-102-1668", "1-281-797-2930",
                      "1-713-575-2157", "1-814-572-2918", "1-619-250-1597", "1-589-665-6563", "1-810-913-9036",
                      "1-397-200-1130", "1-437-680-2407", "1-465-726-9839", "1-307-507-8791", "1-731-657-0191",
                      "1-558-863-6043", "1-541-581-4313", "1-364-370-7529", "1-978-691-6538", "1-571-672-8147",
                      "1-892-156-6093", "1-756-197-0331", "1-199-203-1200", "1-892-667-1661", "1-827-834-8495",
                      "1-407-433-1911", "1-165-309-7970", "1-310-873-0650", "1-971-973-7656", "1-772-275-4303",
                      "1-115-133-1165", "1-482-904-2678", "1-492-431-1471", "1-567-199-1037", "1-509-461-8639",
                      "1-960-849-9079", "1-342-620-4731", "1-206-442-6933", "1-518-906-6287", "1-352-677-7886",
                      "1-852-789-7700", "1-170-155-6412", "1-751-913-7950", "1-922-550-7336", "1-404-362-1043",
                      "1-286-239-1617", "1-799-388-7141", "1-118-182-9537", "1-433-933-7746", "1-289-846-8722",
                      "1-748-743-1169", "1-698-452-0180", "1-501-375-5108", "1-657-363-1516", "1-956-294-4473",
                      "1-517-324-8450", "1-407-886-5584", "1-416-200-4079", "1-934-702-7017", "1-189-762-0900",
                      "1-563-345-6814", "1-430-456-9198", "1-463-741-0282", "1-514-308-7314", "1-636-298-5060",
                      "1-624-147-9287", "1-364-858-8233", "1-113-818-7149", "1-865-282-5786", "1-602-847-9952",
                      "1-918-816-8341", "1-753-837-2723", "1-521-997-0760", "1-472-836-0225", "1-385-208-7887",
                      "1-557-881-3683", "1-259-291-0064", "1-367-212-5437", "1-293-220-0726", "1-661-288-6648",
                      "1-445-484-2203", "1-577-498-2358", "1-692-362-6174", "1-188-449-6817", "1-481-917-7504",
                      "1-942-984-4678", "1-684-903-9049", "1-790-983-0316", "1-898-938-0072", "1-766-562-9613",
                      "1-898-482-9702", "1-456-916-2911", "1-644-695-7572", "1-740-219-3997", "1-234-732-1337",
                      "1-581-966-1805", "1-253-213-2280", "1-646-784-6371", "1-486-606-6295", "1-248-982-9365",
                      "1-901-789-0738", "1-915-151-0829", "1-617-182-7479", "1-282-122-7274", "1-291-852-1896",
                      "1-548-925-6935", "1-722-836-3161", "1-282-401-1283", "1-993-830-4112", "1-121-743-8239",
                      "1-974-760-5444", "1-118-681-7237", "1-335-852-5741", "1-434-628-5460", "1-452-234-4747",
                      "1-262-431-1207", "1-626-621-7666", "1-972-255-5121", "1-825-838-7502", "1-171-174-4485",
                      "1-908-184-0322", "1-979-909-0517", "1-961-873-0688", "1-364-608-6130", "1-758-747-5592",
                      "1-628-139-0592", "1-605-158-6992", "1-766-182-5709", "1-321-358-6488", "1-793-303-4163",
                      "1-352-609-8546", "1-657-362-5824", "1-733-836-7947", "1-897-446-3438", "1-927-416-7556",
                      "1-823-141-6254", "1-855-837-8729", "1-685-459-9630", "1-204-817-7942", "1-480-720-4694",
                      "1-126-688-8913", "1-461-701-8183", "1-907-947-7239", "1-799-507-4001", "1-735-694-6884",
                      "1-934-617-9406", "1-574-809-8164", "1-521-367-5050", "1-713-831-1550", "1-641-447-7761",
                      "1-788-838-4900", "1-376-164-4950", "1-319-872-1531", "1-343-574-3450", "1-987-442-1395",
                      "1-293-775-5382", "1-715-466-1016", "1-136-430-2250", "1-244-406-7032", "1-564-625-2162",
                      "1-526-132-7685", "1-221-494-0011", "1-683-530-1356", "1-681-457-8808", "1-451-862-8460",
                      "1-238-685-1973", "1-915-902-3462", "1-774-260-2604", "1-880-256-6594", "1-319-634-2678",
                      "1-356-754-0432", "1-533-130-7560", "1-329-117-2083", "1-342-851-5231", "1-679-745-0666",
                      "1-412-991-2091", "1-883-873-3782", "1-772-639-5711", "1-785-765-3554", "1-928-573-7524",
                      "1-304-399-7609", "1-459-362-2811", "1-531-311-3813", "1-305-864-5626", "1-477-918-5762",
                      "1-681-501-5125", "1-598-977-6107", "1-335-568-7488", "1-721-103-3548", "1-460-120-3976"}; //200
    char *addresses[] = {"7876 Ipsum Ave", "465-9123 Curae; Rd.", "907-3256 Dapibus Road", "427-6254 Sagittis Ave",
                         "P.O. Box 555, 2263 Dolor, St.", "Ap #799-3364 Nullam Rd.", "Ap #520-306 In Road",
                         "P.O. Box 688, 2167 Malesuada Street", "P.O. Box 373, 7274 Aenean Rd.",
                         "P.O. Box 676, 1439 Lorem, Av.", "P.O. Box 759, 862 Posuere Ave", "Ap #458-4078 Et St.",
                         "P.O. Box 885, 2846 Arcu Street", "P.O. Box 760, 1983 Et Av.", "3522 Ac Road",
                         "6520 Malesuada St.", "P.O. Box 534, 5482 Pellentesque Street", "8698 Ante Rd.",
                         "144-5309 Sit Street", "Ap #280-9049 Consequat Ave", "1805 Laoreet Rd.", "510-5109 Dui. Ave",
                         "P.O. Box 243, 7437 Porta St.", "Ap #254-8964 Magna. Road", "P.O. Box 787, 5066 Nec, St.",
                         "599-4065 Nascetur St.", "2505 Lacinia Rd.", "P.O. Box 966, 5998 Montes, Av.",
                         "4273 Fringilla Ave", "8666 Aliquet Avenue", "1472 Suspendisse St.", "677-6492 Gravida Rd.",
                         "P.O. Box 455, 1767 Vehicula Rd.", "P.O. Box 737, 7786 Feugiat St.", "6395 Amet Rd.",
                         "Ap #479-452 Nibh Av.", "Ap #657-5806 Eget, Street", "6406 Convallis Avenue",
                         "393-1741 Euismod St.", "4208 Mauris Rd.", "1356 Blandit Rd.", "Ap #807-5969 Vel St.",
                         "Ap #539-5603 Maecenas Rd.", "Ap #306-5981 Sed St.", "3445 Cubilia Street",
                         "820-7432 Urna Road", "P.O. Box 312, 8233 A, Rd.", "171-9511 Donec Rd.", "644-7672 Ac Street",
                         "6524 Maecenas Rd.", "3185 Feugiat Ave", "P.O. Box 269, 3920 Vivamus Avenue", "5521 Amet, Ave",
                         "7392 Curae; Rd.", "4146 Sociis Street", "P.O. Box 992, 7472 Leo. Av.",
                         "P.O. Box 227, 6818 Adipiscing Rd.", "Ap #583-9669 Sem, Avenue", "Ap #834-6234 Aliquet Av.",
                         "Ap #957-9955 Pede Avenue", "5108 Vel Rd.", "334-2942 Lacinia Avenue",
                         "P.O. Box 435, 5514 Luctus St.", "8789 Neque Ave", "900-9634 Venenatis Street",
                         "3282 Dictum Rd.", "Ap #619-2016 Nunc Rd.", "3895 Donec Av.", "6792 Feugiat Rd.",
                         "Ap #818-8174 Blandit Rd.", "132 Porttitor Ave", "Ap #752-5071 Eget Ave", "9087 Fermentum St.",
                         "7540 Ornare Road", "P.O. Box 253, 7343 Vel, Rd.", "Ap #912-3321 Eget Av.",
                         "Ap #798-7418 Rhoncus. Ave", "P.O. Box 133, 5998 Semper St.", "700-8294 Sem, Rd.",
                         "8291 Dignissim. Rd.", "551-2856 Vel Road", "5720 Auctor Road", "Ap #456-1238 Etiam Avenue",
                         "P.O. Box 593, 4327 Id Avenue", "423-9170 In, Ave", "P.O. Box 372, 6464 Consectetuer Road",
                         "1665 Pharetra Street", "7464 Amet, Avenue", "352-8669 Ligula Road",
                         "P.O. Box 421, 4487 Ut Rd.", "5928 Eu, St.", "Ap #595-8748 Posuere St.",
                         "871-5018 Ullamcorper, St.", "Ap #586-3278 Dictum Road", "771-6705 Iaculis Rd.",
                         "629-8845 Vel Ave", "Ap #762-1088 Quis Av.", "2455 Nibh. Rd.", "Ap #451-3630 Dolor Rd.",
                         "819-5428 Aliquam Av."}; //100

    const int NUM_OF_NAMES = 100;
    const int NUM_OF_SURNAMES = 100;
    const int NUM_OF_BIRTHDAYS = 100;
    const int NUM_OF_EMAILS = 100;
    const int NUM_OF_PHONES = 200;
    const int NUM_OF_ADDRESSES = 100;
    const int NUM_OF_CONTACTS = 1000;
    
    FILE *file;
    file = fopen("results.txt", "a");

    clock_t real;
    struct rusage usage;

    real = clock();
    getrusage(RUSAGE_SELF, &usage);
    
    fprintf(file, "%s", "First check\n");
    check_times(&real, &usage, file);
    fprintf(file, "%s", "############################# LIST STRUCTURE #############################\n\n");
    fprintf(file, "%s", "Creating Address Book of 1000 random people sorted by surname.\n");

    AddressBook *book = createAddressBook(0);
    fprintf(file, "%s", "Address book created\n");

    check_times(&real, &usage, file);

    for (int i = 0; i < NUM_OF_CONTACTS - 2; ++i) {
        char **data = createData(
                names[rand() % NUM_OF_NAMES],
                surnames[rand() % NUM_OF_SURNAMES],
                birthdays[rand() % NUM_OF_BIRTHDAYS],
                emails[rand() % NUM_OF_EMAILS],
                phones[rand() % NUM_OF_PHONES],
                addresses[rand() % NUM_OF_ADDRESSES]
        );
        addPerson(book, data);
        //printf("%d contact added\n", i + 1);

        //check_times(&real, &usage, file);
    }

    char **data = createData("Bartlomiej", "Pyjor", "1997-24-05", "donkresko@gmail.com", "857365784",
                             "ul. Klikowska 548, 33-100 Tarcurrent");
    addPerson(book, data);
    //printf("999 contact added\n");

    //check_times(&real, &usage, file);

    data = createData("Janusz", "currentak", "1900-24-02", "Janusz.Andrzej.currentak.Kozak@cebulandia.pl", "0-700-880",
                      "ul. Friedleina 205. 33-100 Tarcurrent");
    addPerson(book, data);
    fprintf(file, "%s", "1000 contacts added\n");

    check_times(&real, &usage, file);

    sortByKey(book, 1);
    fprintf(file, "%s", "Address book sorted by birth date.\n");

    check_times(&real, &usage, file);

    searchEl(book, "1900-24-02");
    fprintf(file, "%s", "Optimistic searching.\n");

    check_times(&real, &usage, file);

    removeEl(book, "1900-24-02");
    fprintf(file, "%s", "Optimistic removing.\n");

    check_times(&real, &usage, file);

    searchEl(book, "1997-24-05");
    fprintf(file, "%s", "Pessimistic searching.\n");

    check_times(&real, &usage, file);

    removeEl(book, "1997-24-05");
    fprintf(file, "%s", "Pessimistic removing.\n");

    check_times(&real, &usage, file);

    sortByKey(book, 2);
    fprintf(file, "%s", "Address book sorted by email.\n");

    check_times(&real, &usage, file);

    sortByKey(book, 3);
    fprintf(file, "%s", "Address book sorted by phone number.\n");

    check_times(&real, &usage, file);

    deleteBook(book);
    fprintf(file, "%s", "Deleting address book\n");

    check_times(&real, &usage, file);

    fprintf(file, "%s", "############################# TREE STRUCTURE #############################\n");
    fprintf(file, "%s", "Creating Address Book of 1000 random people sorted by surname.\n");

    book = createAddressBook(4);
    fprintf(file, "%s", "Address book created\n");

    check_times(&real, &usage, file);

    for (int i = 0; i < NUM_OF_CONTACTS - 2; ++i) {
        data = createData(
                names[rand() % NUM_OF_NAMES],
                surnames[rand() % NUM_OF_SURNAMES],
                birthdays[rand() % NUM_OF_BIRTHDAYS],
                emails[rand() % NUM_OF_EMAILS],
                phones[rand() % NUM_OF_PHONES],
                addresses[rand() % NUM_OF_ADDRESSES]
        );
        addPerson(book, data);
        //printf("%d contact added\n", i + 1);

        //check_times(&real, &usage, file);
    }

    data = createData("Bartlomiej", "Pyjor", "1997-24-05", "donkresko@gmail.com", "857365784",
                             "ul. Klikowska 548, 33-100 Tarcurrent");
    addPerson(book, data);
    //printf("999 contact added\n");

    //check_times(&real, &usage, file);

    data = createData("Janusz", "currentak", "1900-24-02", "Janusz.Andrzej.currentak.Kozak@cebulandia.pl", "0-700-880",
                      "ul. Friedleina 205. 33-100 Tarcurrent");
    addPerson(book, data);
    fprintf(file, "%s", "1000 contacts added\n");

    check_times(&real, &usage, file);
    sortByKey(book, 1);
    fprintf(file, "%s", "Address book sorted by birth date.\n");

    check_times(&real, &usage, file);

    BTreeNode *bTreeNode = returnRoot(book);

    real = clock();
    getrusage(RUSAGE_SELF, &usage);

    searchEl(book, bTreeNode->data[1]);
    fprintf(file, "%s", "Optimistic searching.\n");

    check_times(&real, &usage, file);

    removeEl(book, bTreeNode->data[1]);
    fprintf(file, "%s", "Optimistic removing.\n");

    check_times(&real, &usage, file);

    searchEl(book, "1997-24-05");
    fprintf(file, "%s", "Pessimistic searching.\n");

    check_times(&real, &usage, file);

    removeEl(book, "1997-24-05");
    fprintf(file, "%s", "Pessimistic removing.\n");

    check_times(&real, &usage, file);

    sortByKey(book, 2);
    fprintf(file, "%s", "Address book sorted by email.\n");

    check_times(&real, &usage, file);

    sortByKey(book, 3);
    fprintf(file, "%s", "Address book sorted by phone number.\n");

    check_times(&real, &usage, file);

    deleteBook(book);
    fprintf(file, "%s", "Deleting address book\n");

    check_times(&real, &usage, file);

    //printBook(book);
    
    fprintf(file, "%s", "END OF PROGRAM\n\n\n");
    
    fclose(file);

}



void check_times(clock_t *real, struct rusage *usage, FILE *file) {
    clock_t curr_real;
    struct rusage curr_usage;

    curr_real = clock();
    getrusage(RUSAGE_SELF, &curr_usage);

    fprintf(file, "Current time:\t\tR:\t%.6f\t\tU:\t%.6f\t\tS:\t%.6f\n",
           (double)(curr_real)/CLOCKS_PER_SEC,
           curr_usage.ru_utime.tv_sec + curr_usage.ru_utime.tv_usec / 10e6,
           curr_usage.ru_stime.tv_sec + curr_usage.ru_stime.tv_usec / 10e6);

    fprintf(file, "From prev check:\tR:\t%.6f\t\tU:\t%.6f\t\tS:\t%.6f\n\n",
           (double)(curr_real -(*real))/CLOCKS_PER_SEC,
           (curr_usage.ru_utime.tv_sec - (*usage).ru_utime.tv_sec) +
           (curr_usage.ru_utime.tv_usec - (*usage).ru_utime.tv_usec) / 10e6,
           (curr_usage.ru_stime.tv_sec - (*usage).ru_stime.tv_sec) +
           (curr_usage.ru_stime.tv_usec - (*usage).ru_stime.tv_usec) / 10e6);

    (*real) = curr_real;
    (*usage) = curr_usage;
}
