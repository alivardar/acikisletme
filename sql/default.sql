



--
-- PostgreSQL database dump
--


CREATE USER acikisletme with CREATEDB NOCREATEUSER;

SET SESSION AUTHORIZATION 'acikisletme';

--
-- TOC entry 3 (OID 2200)
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
GRANT ALL ON SCHEMA public TO PUBLIC;


SET SESSION AUTHORIZATION 'acikisletme';

SET search_path = public, pg_catalog;

--
-- TOC entry 28 (OID 19033)
-- Name: pga_queries; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_queries (
    queryname character varying(64),
    querytype character(1),
    querycommand text,
    querytables text,
    querylinks text,
    queryresults text,
    querycomments text
);


--
-- TOC entry 29 (OID 19033)
-- Name: pga_queries; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_queries FROM PUBLIC;
GRANT ALL ON TABLE pga_queries TO PUBLIC;


--
-- TOC entry 30 (OID 19038)
-- Name: pga_forms; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_forms (
    formname character varying(64),
    formsource text
);


--
-- TOC entry 31 (OID 19038)
-- Name: pga_forms; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_forms FROM PUBLIC;
GRANT ALL ON TABLE pga_forms TO PUBLIC;


--
-- TOC entry 32 (OID 19043)
-- Name: pga_scripts; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_scripts (
    scriptname character varying(64),
    scriptsource text
);


--
-- TOC entry 33 (OID 19043)
-- Name: pga_scripts; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_scripts FROM PUBLIC;
GRANT ALL ON TABLE pga_scripts TO PUBLIC;


--
-- TOC entry 34 (OID 19048)
-- Name: pga_reports; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_reports (
    reportname character varying(64),
    reportsource text,
    reportbody text,
    reportprocs text,
    reportoptions text
);


--
-- TOC entry 35 (OID 19048)
-- Name: pga_reports; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_reports FROM PUBLIC;
GRANT ALL ON TABLE pga_reports TO PUBLIC;


--
-- TOC entry 36 (OID 19053)
-- Name: pga_schema; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_schema (
    schemaname character varying(64),
    schematables text,
    schemalinks text
);


--
-- TOC entry 37 (OID 19053)
-- Name: pga_schema; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_schema FROM PUBLIC;
GRANT ALL ON TABLE pga_schema TO PUBLIC;


--
-- TOC entry 4 (OID 19058)
-- Name: faturagelen_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE faturagelen_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 6 (OID 19065)
-- Name: firmalar_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE firmalar_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 8 (OID 19072)
-- Name: kullanicilar_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE kullanicilar_row_no_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 38 (OID 19074)
-- Name: kullanicilar; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE kullanicilar (
    row_no bigint DEFAULT nextval('"kullanicilar_row_no_seq"'::text) NOT NULL,
    kullanici character varying(10) DEFAULT ''::character varying NOT NULL,
    sifre character varying(100) DEFAULT ''::character varying NOT NULL,
    haklar character varying(64)
);


--
-- TOC entry 10 (OID 19079)
-- Name: makbuz_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE makbuz_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 12 (OID 19090)
-- Name: sirket_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE sirket_row_no_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 14 (OID 19092)
-- Name: stokgelen_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE stokgelen_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 16 (OID 19099)
-- Name: stokgiden_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE stokgiden_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 18 (OID 19107)
-- Name: stokkart_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE stokkart_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 20 (OID 19114)
-- Name: faturakesilen_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE faturakesilen_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 22 (OID 19120)
-- Name: faturalar_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE faturalar_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 24 (OID 19133)
-- Name: irsaliyeler_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE irsaliyeler_row_no_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 39 (OID 19146)
-- Name: sirket; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE sirket (
    row_no bigint NOT NULL,
    sirket_adi character varying(100) DEFAULT ''::character varying NOT NULL,
    adres character varying(200) DEFAULT ''::character varying NOT NULL,
    vergi_dairesi character varying(50),
    vergi_no character varying(50),
    telefon1 character varying(30),
    telefon2 character varying(30),
    fax character varying(30)
);


--
-- TOC entry 40 (OID 19150)
-- Name: pga_layout; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE pga_layout (
    tablename character varying(64),
    nrcols smallint,
    colnames text,
    colwidth text
);


--
-- TOC entry 41 (OID 19150)
-- Name: pga_layout; Type: ACL; Schema: public; Owner: acikisletme
--

REVOKE ALL ON TABLE pga_layout FROM PUBLIC;
GRANT ALL ON TABLE pga_layout TO PUBLIC;


--
-- TOC entry 26 (OID 19158)
-- Name: ceksenet_row_no_seq; Type: SEQUENCE; Schema: public; Owner: acikisletme
--

CREATE SEQUENCE ceksenet_row_no_seq
    START WITH 1
    INCREMENT BY 1
    MAXVALUE 2147483647
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 42 (OID 27439)
-- Name: ceksenet; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE ceksenet (
    row_no bigint DEFAULT nextval('"irsaliyeler_row_no_seq"'::text) NOT NULL,
    belge_no character varying(30) NOT NULL,
    islem_tarih date NOT NULL,
    firma_adi character varying(100) NOT NULL,
    aciklama character varying(150) NOT NULL,
    tutar double precision NOT NULL,
    banka_adi character varying(50) NOT NULL,
    cek_no character varying(30) NOT NULL,
    cek_tarih date NOT NULL,
    keside_yer character varying(20) NOT NULL,
    cek_kesen_firma character varying(30) NOT NULL,
    durum character varying(6) NOT NULL,
    cikan_firma_adi character varying(100)
);


--
-- TOC entry 43 (OID 27442)
-- Name: faturagelen; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE faturagelen (
    row_no bigint DEFAULT nextval('"faturagelen_row_no_seq"'::text) NOT NULL,
    tarih date,
    malzeme_kodu character varying(50) DEFAULT ''::character varying NOT NULL,
    belge_no character varying(100),
    irsaliye_no character varying(100) DEFAULT ''::character varying NOT NULL,
    aciklama character varying(150),
    birim_fiyat double precision,
    miktar character varying(20),
    kimden character varying(100),
    kdv_oran character varying(30),
    kdv_tutar double precision,
    birim_tip character varying(10),
    tutar double precision
);


--
-- TOC entry 44 (OID 27455)
-- Name: faturakesilen; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE faturakesilen (
    row_no bigint DEFAULT nextval('"faturakesilen_row_no_seq"'::text) NOT NULL,
    tarih date,
    malzeme_kodu character varying(50) DEFAULT ''::character varying NOT NULL,
    belge_no character varying(100),
    irsaliye_no character varying(100),
    aciklama character varying(150),
    birim_fiyat character varying(50),
    miktar character varying(20),
    kimden character varying(100),
    kdv_oran character varying(3),
    kdv_tutar double precision,
    birim_tip character varying(6),
    tutar double precision
);


--
-- TOC entry 45 (OID 27470)
-- Name: faturalar; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE faturalar (
    row_no bigint DEFAULT nextval('"faturalar_row_no_seq"'::text) NOT NULL,
    tarih date DEFAULT '0001-01-01'::date NOT NULL,
    fatura_no character varying(50) DEFAULT ''::character varying NOT NULL,
    irsaliye_no character varying(50) DEFAULT ''::character varying NOT NULL,
    firma_adi character varying(50) DEFAULT ''::character varying NOT NULL,
    toplam double precision NOT NULL,
    kdv double precision NOT NULL,
    toplam_tutar double precision NOT NULL,
    tur character varying(5) DEFAULT ''::character varying NOT NULL,
    isk_oran character varying(3),
    isk_tutar double precision,
    duzenleyen character varying(20)
);


--
-- TOC entry 46 (OID 27478)
-- Name: firmalar; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE firmalar (
    row_no bigint DEFAULT nextval('"firmalar_row_no_seq"'::text) NOT NULL,
    bilanco_no character varying(100),
    firma_adi character varying(100) DEFAULT ''::character varying NOT NULL,
    adres character varying(200),
    vergi_dairesi character varying(50),
    vergi_no character varying(50),
    telefon character varying(100),
    fax character varying(50),
    aciklama character varying(100),
    bakiye double precision NOT NULL,
    durum bpchar,
    para_birim character varying(10)
);


--
-- TOC entry 47 (OID 27487)
-- Name: irsaliyeler; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE irsaliyeler (
    row_no bigint DEFAULT nextval('"irsaliyeler_row_no_seq"'::text) NOT NULL,
    tarih date DEFAULT '0001-01-01'::date NOT NULL,
    fatura_no character varying(50) DEFAULT ''::character varying NOT NULL,
    irsaliye_no character varying(50) DEFAULT ''::character varying NOT NULL,
    firma_adi character varying(50) DEFAULT ''::character varying NOT NULL,
    toplam double precision NOT NULL,
    kdv double precision NOT NULL,
    toplam_tutar double precision NOT NULL,
    tur character varying(5) DEFAULT ''::character varying NOT NULL,
    talan character varying(20),
    teden character varying(20),
    fsevktar character varying(10),
    duz_saat character varying(10),
    teslim_adresi character varying(50),
    duzenleyen character varying(20)
);


--
-- TOC entry 48 (OID 27495)
-- Name: makbuz; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE makbuz (
    row_no bigint DEFAULT nextval('"makbuz_row_no_seq"'::text) NOT NULL,
    belge_no character varying(30) DEFAULT ''::character varying NOT NULL,
    tarih date DEFAULT '0001-01-01'::date NOT NULL,
    firma_adi character varying(100) DEFAULT ''::character varying NOT NULL,
    aciklama character varying(150) DEFAULT ''::character varying NOT NULL,
    tutar double precision NOT NULL,
    tur character varying(6) DEFAULT 'tahsil'::character varying NOT NULL
);


--
-- TOC entry 49 (OID 27503)
-- Name: stokgelen; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE stokgelen (
    row_no bigint DEFAULT nextval('"stokgelen_row_no_seq"'::text) NOT NULL,
    tarih date,
    malzeme_kodu character varying(50) DEFAULT ''::character varying NOT NULL,
    belge_no character varying(100),
    aciklama character varying(150),
    birim_fiyat double precision,
    gelen character varying(20),
    kimden character varying(100),
    tur bpchar DEFAULT ''::bpchar NOT NULL,
    fatura_no character varying(50),
    birim_tip character varying(10),
    talan character varying(20),
    teden character varying(20)
);


--
-- TOC entry 50 (OID 27513)
-- Name: stokgiden; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE stokgiden (
    row_no bigint DEFAULT nextval('"stokgiden_row_no_seq"'::text) NOT NULL,
    tarih date,
    malzeme_kodu character varying(50) DEFAULT ''::character varying NOT NULL,
    belge_no character varying(100),
    aciklama character varying(150),
    birim_fiyat double precision,
    giden character varying(20),
    kime character varying(100),
    fatura_no character varying(30) DEFAULT ''::character varying NOT NULL,
    tur bpchar DEFAULT ''::bpchar NOT NULL,
    birim_tip character varying(10),
    talan character varying(20),
    teden character varying(20)
);


--
-- TOC entry 51 (OID 27531)
-- Name: stokkart; Type: TABLE; Schema: public; Owner: acikisletme
--

CREATE TABLE stokkart (
    row_no bigint DEFAULT nextval('"stokkart_row_no_seq"'::text) NOT NULL,
    malzeme_kodu character varying(50) DEFAULT ''::character varying NOT NULL,
    malzeme_adi character varying(100) DEFAULT ''::character varying NOT NULL,
    aciklama character varying(150),
    seri_no character varying(50),
    birim_fiyat double precision,
    mevcut character varying(20),
    kdv_orani character varying(2),
    birim_tip character varying(4)
);


--
-- Data for TOC entry 61 (OID 19033)
-- Name: pga_queries; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_queries (queryname, querytype, querycommand, querytables, querylinks, queryresults, querycomments) FROM stdin;
\.


--
-- Data for TOC entry 62 (OID 19038)
-- Name: pga_forms; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_forms (formname, formsource) FROM stdin;
\.


--
-- Data for TOC entry 63 (OID 19043)
-- Name: pga_scripts; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_scripts (scriptname, scriptsource) FROM stdin;
\.


--
-- Data for TOC entry 64 (OID 19048)
-- Name: pga_reports; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_reports (reportname, reportsource, reportbody, reportprocs, reportoptions) FROM stdin;
\.


--
-- Data for TOC entry 65 (OID 19053)
-- Name: pga_schema; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_schema (schemaname, schematables, schemalinks) FROM stdin;
\.


--
-- Data for TOC entry 66 (OID 19074)
-- Name: kullanicilar; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY kullanicilar (row_no, kullanici, sifre, haklar) FROM stdin;
0	root	$1$DzMax1X.$bNpeYApr3YiLsB0QM/GDl/	11111111111111111111111111111111111111111111111111111111111111
2	murat	$1$LeHA.x5E$Hu1W/H.b1nShe8cnU9fF21	00000
4	ali	$1$cfyt.LIE$iC7zVCXMHGSSKgJKAgubh/	11111
\.


--
-- Data for TOC entry 67 (OID 19146)
-- Name: sirket; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY sirket (row_no, sirket_adi, adres, vergi_dairesi, vergi_no, telefon1, telefon2, fax) FROM stdin;
0	Linux iþletme bilgisayar yazýlým A.Þ.	Girne cad. þeref sok. no:55 Kadýkoy / istanbul	Kadýkoy V.D.	9860019003456	(0 216) 454 65 67	(0 216) 454 65 68	(0 216) 454 65 69
\.


--
-- Data for TOC entry 68 (OID 19150)
-- Name: pga_layout; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY pga_layout (tablename, nrcols, colnames, colwidth) FROM stdin;
faturagelen	13	row_no tarih malzeme_kodu belge_no irsaliye_no aciklama birim_fiyat miktar kimden kdv_oran kdv_tutar birim_tip tutar	150 150 150 150 150 150 150 150 150 150 150 150 150
faturakesilen	13	row_no tarih malzeme_kodu belge_no irsaliye_no aciklama birim_fiyat miktar kimden kdv_oran kdv_tutar birim_tip tutar	150 150 150 150 150 150 150 150 150 150 150 150 150
kullanicilar	4	row_no kullanici sifre haklar	150 150 150 150
v1	1	malzeme_kodu	150
stokkart	9	row_no malzeme_kodu malzeme_adi aciklama seri_no birim_fiyat mevcut kdv_orani birim_tip	65 150 78 50 77 85 69 56 55
makbuz	7	row_no belge_no tarih firma_adi aciklama tutar tur	79 150 150 150 150 150 150
stokgelen	13	row_no tarih malzeme_kodu belge_no aciklama birim_fiyat gelen kimden tur fatura_no birim_tip {talan} {teden}	59 150 70 150 150 85 76 59 150 150 150 150 150
stokgiden	13	row_no tarih malzeme_kodu belge_no aciklama birim_fiyat giden kime fatura_no tur birim_tip {talan} {teden}	55 60 98 98 95 87 75 94 77 83 150 150 150
faturalar	12	row_no tarih fatura_no irsaliye_no firma_adi toplam kdv toplam_tutar tur isk_oran isk_tutar {duzenleyen}	150 150 150 150 150 150 150 150 150 150 150 150
irsaliyeler	15	row_no tarih fatura_no irsaliye_no firma_adi toplam kdv toplam_tutar tur {talan} {teden} {fsevktar} {duz_saat} {teslim_adresi} {duzenleyen}	53 46 7 78 95 49 54 58 60 56 80 80 66 10 150
firmalar	12	row_no bilanco_no firma_adi adres vergi_dairesi vergi_no telefon fax aciklama bakiye durum {para_birim}	70 72 92 96 81 84 91 73 87 60 150 150
ceksenet	13	row_no belge_no islem_tarih firma_adi aciklama tutar banka_adi cek_no cek_tarih keside_yer cek_kesen_firma durum {cikan_firma_adi}	47 52 150 150 82 60 85 70 69 57 67 150 150
nolayoutneeded	1	islem_tarih	150
	13	row_no belge_no islem_tarih firma_adi aciklama tutar banka_adi cek_no cek_tarih keside_yer cek_kesen_firma durum cikan_firma_adi	150 150 150 150 150 150 150 150 150 150 150 150 150
sirket	8	row_no sirket_adi adres vergi_dairesi vergi_no telefon1 telefon2 fax	150 150 150 150 150 150 150 150
\.


--
-- Data for TOC entry 69 (OID 27439)
-- Name: ceksenet; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY ceksenet (row_no, belge_no, islem_tarih, firma_adi, aciklama, tutar, banka_adi, cek_no, cek_tarih, keside_yer, cek_kesen_firma, durum, cikan_firma_adi) FROM stdin;
15	2505	2004-08-18	212	retret	250	324324	234324	5000-04-01	01	04	sirket	\N
\.


--
-- Data for TOC entry 70 (OID 27442)
-- Name: faturagelen; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY faturagelen (row_no, tarih, malzeme_kodu, belge_no, irsaliye_no, aciklama, birim_fiyat, miktar, kimden, kdv_oran, kdv_tutar, birim_tip, tutar) FROM stdin;
\.


--
-- Data for TOC entry 71 (OID 27455)
-- Name: faturakesilen; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY faturakesilen (row_no, tarih, malzeme_kodu, belge_no, irsaliye_no, aciklama, birim_fiyat, miktar, kimden, kdv_oran, kdv_tutar, birim_tip, tutar) FROM stdin;
\.


--
-- Data for TOC entry 72 (OID 27470)
-- Name: faturalar; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY faturalar (row_no, tarih, fatura_no, irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, tur, isk_oran, isk_tutar, duzenleyen) FROM stdin;
7	2004-11-27	12545		212	5454	981	6435	1	10	606	\N
\.


--
-- Data for TOC entry 73 (OID 27478)
-- Name: firmalar; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY firmalar (row_no, bilanco_no, firma_adi, adres, vergi_dairesi, vergi_no, telefon, fax, aciklama, bakiye, durum, para_birim) FROM stdin;
8	1	212	12121	21212	12121	21212	121212	121212	5610	B	TL
\.


--
-- Data for TOC entry 74 (OID 27487)
-- Name: irsaliyeler; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY irsaliyeler (row_no, tarih, fatura_no, irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, tur, talan, teden, fsevktar, duz_saat, teslim_adresi, duzenleyen) FROM stdin;
\.


--
-- Data for TOC entry 75 (OID 27495)
-- Name: makbuz; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY makbuz (row_no, belge_no, tarih, firma_adi, aciklama, tutar, tur) FROM stdin;
5	10	2004-08-18	212	212313	100000	tahsil
\.


--
-- Data for TOC entry 76 (OID 27503)
-- Name: stokgelen; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY stokgelen (row_no, tarih, malzeme_kodu, belge_no, aciklama, birim_fiyat, gelen, kimden, tur, fatura_no, birim_tip, talan, teden) FROM stdin;
\.


--
-- Data for TOC entry 77 (OID 27513)
-- Name: stokgiden; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY stokgiden (row_no, tarih, malzeme_kodu, belge_no, aciklama, birim_fiyat, giden, kime, fatura_no, tur, birim_tip, talan, teden) FROM stdin;
\.


--
-- Data for TOC entry 78 (OID 27531)
-- Name: stokkart; Type: TABLE DATA; Schema: public; Owner: acikisletme
--

COPY stokkart (row_no, malzeme_kodu, malzeme_adi, aciklama, seri_no, birim_fiyat, mevcut, kdv_orani, birim_tip) FROM stdin;
5	1	11112	121212	1212	1212	12	18	Adet
\.


--
-- TOC entry 53 (OID 19222)
-- Name: kullanicilar_row_no_key; Type: INDEX; Schema: public; Owner: acikisletme
--

CREATE UNIQUE INDEX kullanicilar_row_no_key ON kullanicilar USING btree (row_no);


--
-- TOC entry 52 (OID 19231)
-- Name: kullanicilar_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY kullanicilar
    ADD CONSTRAINT kullanicilar_pkey PRIMARY KEY (kullanici);


--
-- TOC entry 54 (OID 19241)
-- Name: sirket_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY sirket
    ADD CONSTRAINT sirket_pkey PRIMARY KEY (row_no);


--
-- TOC entry 55 (OID 27447)
-- Name: faturagelen_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY faturagelen
    ADD CONSTRAINT faturagelen_pkey PRIMARY KEY (row_no);


--
-- TOC entry 56 (OID 27459)
-- Name: faturakesilen_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY faturakesilen
    ADD CONSTRAINT faturakesilen_pkey PRIMARY KEY (row_no);


--
-- TOC entry 57 (OID 27485)
-- Name: firmalar_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY firmalar
    ADD CONSTRAINT firmalar_pkey PRIMARY KEY (row_no);


--
-- TOC entry 58 (OID 27511)
-- Name: stokgelen_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY stokgelen
    ADD CONSTRAINT stokgelen_pkey PRIMARY KEY (row_no);


--
-- TOC entry 59 (OID 27522)
-- Name: stokgiden_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY stokgiden
    ADD CONSTRAINT stokgiden_pkey PRIMARY KEY (row_no);


--
-- TOC entry 60 (OID 27536)
-- Name: stokkart_pkey; Type: CONSTRAINT; Schema: public; Owner: acikisletme
--

ALTER TABLE ONLY stokkart
    ADD CONSTRAINT stokkart_pkey PRIMARY KEY (row_no);


--
-- TOC entry 5 (OID 19058)
-- Name: faturagelen_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('faturagelen_row_no_seq', 6, true);


--
-- TOC entry 7 (OID 19065)
-- Name: firmalar_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('firmalar_row_no_seq', 8, true);


--
-- TOC entry 9 (OID 19072)
-- Name: kullanicilar_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('kullanicilar_row_no_seq', 1, false);


--
-- TOC entry 11 (OID 19079)
-- Name: makbuz_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('makbuz_row_no_seq', 5, true);


--
-- TOC entry 13 (OID 19090)
-- Name: sirket_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('sirket_row_no_seq', 1, false);


--
-- TOC entry 15 (OID 19092)
-- Name: stokgelen_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('stokgelen_row_no_seq', 6, true);


--
-- TOC entry 17 (OID 19099)
-- Name: stokgiden_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('stokgiden_row_no_seq', 8, true);


--
-- TOC entry 19 (OID 19107)
-- Name: stokkart_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('stokkart_row_no_seq', 5, true);


--
-- TOC entry 21 (OID 19114)
-- Name: faturakesilen_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('faturakesilen_row_no_seq', 8, true);


--
-- TOC entry 23 (OID 19120)
-- Name: faturalar_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('faturalar_row_no_seq', 7, true);


--
-- TOC entry 25 (OID 19133)
-- Name: irsaliyeler_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('irsaliyeler_row_no_seq', 15, true);


--
-- TOC entry 27 (OID 19158)
-- Name: ceksenet_row_no_seq; Type: SEQUENCE SET; Schema: public; Owner: acikisletme
--

SELECT pg_catalog.setval('ceksenet_row_no_seq', 1, false);


SET SESSION AUTHORIZATION 'postgres';

--
-- TOC entry 2 (OID 2200)
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'Standard public schema';


